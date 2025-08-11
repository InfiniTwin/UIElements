#include "ItemViewsFeature.h"
#include "ECS.h"
#include "Engine/Engine.h"

namespace UI {
	using namespace ECS;

	static TSharedRef<ITableRow> GenerateRow_Default(
		TSharedPtr<FEntityItem> item,
		const TSharedRef<STableViewBase>& owner)
	{
		return StaticCastSharedRef<ITableRow>(
			SNew(STableRow<TSharedPtr<FEntityItem>>, owner)
			.Content()
			[
				SNew(STextBlock)
					.Text(FText::FromString(FString(UTF8_TO_TCHAR(item->Entity.name()))))
			]
		);
	}

	static flecs::entity FindTreeRoot(flecs::entity e) {
		for (flecs::entity p = e.parent(); p.is_valid(); p = p.parent())
			if (p.has<TreeRoot>())
				return p;
		return flecs::entity();
	}

	static void RebuildChildrenRecursive(TreeViewState* state, flecs::entity e, const TSharedPtr<FEntityItem>& node)
	{
		node->Children.Reset();

		TArray<TPair<int, flecs::entity_t>> sorted;
		e.children([&](flecs::entity c) {
			if (!c.has<TreeItem>()) return;
			if (c.has<Visibility>(Hidden) || c.has<Visibility>(Collapsed)) return;
			int order = 0; if (auto* p = c.try_get<Order>()) order = p->Value;
			sorted.Add({ order, c.id() });
			});
		sorted.StableSort([](auto& a, auto& b) { return a.Key < b.Key; });

		for (auto& it : sorted) {
			flecs::entity ch = flecs::entity(e.world(), it.Value);
			TSharedPtr<FEntityItem> cp = state->GetPtr(ch);
			node->Children.Add(cp);
			RebuildChildrenRecursive(state, ch, cp);
		}
	}

	static void RebuildRoots(flecs::entity tree)
	{
		auto* state = tree.try_get_mut<TreeViewState>();
		if (!state) return;
		if (!state->Roots.IsValid())
			state->Roots = MakeShared<TArray<TSharedPtr<FEntityItem>>>();

		TArray<TSharedPtr<FEntityItem>>& roots = *state->Roots;
		roots.Reset();

		TArray<TPair<int, flecs::entity_t>> sorted;
		tree.children([&](flecs::entity c) {
			if (!c.has<TreeItem>()) return;
			if (c.has<Visibility>(Hidden) || c.has<Visibility>(Collapsed)) return;
			int order = 0; if (auto* p = c.try_get<Order>()) order = p->Value;
			sorted.Add({ order, c.id() });
			});
		sorted.StableSort([](auto& a, auto& b) { return a.Key < b.Key; });

		for (auto& it : sorted) {
			flecs::entity r = flecs::entity(tree.world(), it.Value);
			TSharedPtr<FEntityItem> rp = state->GetPtr(r);
			roots.Add(rp);
			RebuildChildrenRecursive(state, r, rp);
		}

		if (auto* inst = tree.try_get<WidgetInstance>()) {
			if (inst->Value.IsValid()) {
				TSharedRef<SWidget> w = inst->Value.ToSharedRef();
				StaticCastSharedRef<STreeView<TSharedPtr<FEntityItem>>>(w)->RequestTreeRefresh();
			}
		}
	}

	void ItemViewsFeature::RegisterComponents(flecs::world& world) {
		world.component<TreeRoot>().add(flecs::OnInstantiate, flecs::Inherit);
		world.component<TreeItem>().add(flecs::OnInstantiate, flecs::Inherit);
		world.component<TreeRowContent>().add(flecs::OnInstantiate, flecs::Inherit);
		world.component<Expanded>().add(flecs::CanToggle);
		world.component<Selected>().add(flecs::CanToggle);
		world.component<ItemLabel>().member<FText>(VALUE).add(flecs::OnInstantiate, flecs::Inherit);
		world.component<TreeViewState>();
	}

	void ItemViewsFeature::CreateObservers(flecs::world& world) {
		world.observer<>("AddTreeViewInstance")
			.with<Widget>()
			.with<TreeRoot>()
			.event(flecs::OnAdd)
			.each([](flecs::entity tree) {
			auto* state = tree.try_get_mut<TreeViewState>();
			if (!state) return;
			if (!state->Roots.IsValid())
				state->Roots = MakeShared<TArray<TSharedPtr<FEntityItem>>>();

			TArray<TSharedPtr<FEntityItem>>* roots_ptr = state->Roots.Get();

			TSharedRef<STreeView<TSharedPtr<FEntityItem>>> view =
				SNew(STreeView<TSharedPtr<FEntityItem>>)
				.TreeItemsSource(roots_ptr)
				.OnGenerateRow_Lambda([](TSharedPtr<FEntityItem> item, const TSharedRef<STableViewBase>& owner)->TSharedRef<ITableRow> {
				flecs::entity e = item->Entity;
				TSharedPtr<SWidget> row_widget;
				e.children([&](flecs::entity c) {
					if (c.has<TreeRowContent>()) {
						if (auto* wi = c.try_get<WidgetInstance>())
							row_widget = wi->Value;
					}
					});
				if (row_widget.IsValid()) {
					return StaticCastSharedRef<ITableRow>(
						SNew(STableRow<TSharedPtr<FEntityItem>>, owner)
						.Content()[row_widget.ToSharedRef()]
					);
				}
				return GenerateRow_Default(item, owner);
					})
				.OnGetChildren_Lambda([](TSharedPtr<FEntityItem> item, TArray<TSharedPtr<FEntityItem>>& out) {
				out = item->Children;
					})
				.OnExpansionChanged_Lambda([](TSharedPtr<FEntityItem> item, bool expanded) {
				if (expanded) item->Entity.add<Expanded>(); else item->Entity.remove<Expanded>();
					});

			tree.set(WidgetInstance{ view });
			tree.add<Attached>().disable<Attached>();

			RebuildRoots(tree);

			auto* st = tree.try_get_mut<TreeViewState>();
			UE_LOG(LogTemp, Warning, TEXT("Tree %s: roots=%d"), UTF8_TO_TCHAR(tree.name()), st && st->Roots.IsValid() ? st->Roots->Num() : -1);

			auto weak = TWeakPtr<STreeView<TSharedPtr<FEntityItem>>>(view);
			view->RegisterActiveTimer(0.f, FWidgetActiveTimerDelegate::CreateLambda([state, weak](double, double) {
				TSharedPtr<STreeView<TSharedPtr<FEntityItem>>> tv = weak.Pin();
				if (!tv.IsValid() || !state || !state->Roots.IsValid()) return EActiveTimerReturnType::Stop;
				for (const TSharedPtr<FEntityItem>& r : *state->Roots)
					tv->SetItemExpansion(r, true);
				return EActiveTimerReturnType::Stop;
				}));
				});

		auto refresh = [&](flecs::entity changed) {
			if (auto root = FindTreeRoot(changed); root.is_valid())
				RebuildRoots(root);
			};

		world.observer<>("RefreshTreeOnHierarchyChange_Add")
			.with<TreeItem>()
			.with(flecs::ChildOf, flecs::Wildcard)
			.event(flecs::OnAdd)
			.each([&](flecs::entity e) { refresh(e); });

		world.observer<>("RefreshTreeOnHierarchyChange_Remove")
			.with<TreeItem>()
			.with(flecs::ChildOf, flecs::Wildcard)
			.event(flecs::OnRemove)
			.each([&](flecs::entity e) { refresh(e); });

		world.observer<>("RefreshTreeOnHierarchyChange_Set")
			.with<TreeItem>()
			.with(flecs::ChildOf, flecs::Wildcard)
			.event(flecs::OnSet)
			.each([&](flecs::entity e) { refresh(e); });

		world.observer<>("SyncExpansionToSlate")
			.with<Expanded>()
			.with<TreeItem>()
			.event(flecs::OnAdd)
			.each([](flecs::entity e) {
			flecs::entity cursor = e.parent();
			while (cursor.is_valid() && !cursor.has<TreeRoot>()) cursor = cursor.parent();
			if (!cursor.is_valid()) return;
			if (auto* inst = cursor.try_get<WidgetInstance>()) {
				if (inst->Value.IsValid()) {
					auto* st = cursor.try_get_mut<TreeViewState>();
					if (!st) return;
					TSharedRef<SWidget> w = inst->Value.ToSharedRef();
					auto tv = StaticCastSharedRef<STreeView<TSharedPtr<FEntityItem>>>(w);
					tv->SetItemExpansion(st->GetPtr(e), true);
				}
			}
				});

		world.observer<>("SyncExpansionToSlate_Remove")
			.with<Expanded>()
			.with<TreeItem>()
			.event(flecs::OnRemove)
			.each([](flecs::entity e) {
			flecs::entity cursor = e.parent();
			while (cursor.is_valid() && !cursor.has<TreeRoot>()) cursor = cursor.parent();
			if (!cursor.is_valid()) return;
			if (auto* inst = cursor.try_get<WidgetInstance>()) {
				if (inst->Value.IsValid()) {
					auto* st = cursor.try_get_mut<TreeViewState>();
					if (!st) return;
					TSharedRef<SWidget> w = inst->Value.ToSharedRef();
					auto tv = StaticCastSharedRef<STreeView<TSharedPtr<FEntityItem>>>(w);
					tv->SetItemExpansion(st->GetPtr(e), false);
				}
			}
				});
	}

	void ItemViewsFeature::CreateSystems(flecs::world& world) {
	}

}
