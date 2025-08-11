#pragma once

#include "flecs.h"
#include "WidgetFeature.h"
#include "Widgets/Views/STreeView.h"
#include "Widgets/Views/STableRow.h"
#include "Widgets/Views/STableViewBase.h"
#include "Widgets/Text/STextBlock.h"

namespace UI {

	struct ItemViewsFeature {
		static void RegisterComponents(flecs::world& world);
		static void CreateObservers(flecs::world& world);
		static void CreateSystems(flecs::world& world);
	};

	struct TreeRoot {};
	struct TreeItem {};
	struct TreeRowContent {};
	struct Expanded {};
	struct Selected {};
	struct ItemLabel { FText Value; };

	struct FEntityItem {
		flecs::entity Entity;
		TArray<TSharedPtr<FEntityItem>> Children;
		FEntityItem() : Entity() {}
		explicit FEntityItem(flecs::entity in) : Entity(in) {}
	};

	struct TreeViewState {
		TSharedPtr<TArray<TSharedPtr<FEntityItem>>> Roots;
		TMap<flecs::entity_t, TSharedPtr<FEntityItem>> Cache;
		TSharedPtr<FEntityItem> GetPtr(flecs::entity e) {
			const flecs::entity_t id = e.id();
			if (TSharedPtr<FEntityItem>* found = Cache.Find(id)) return *found;
			TSharedPtr<FEntityItem> p = MakeShared<FEntityItem>(e);
			Cache.Add(id, p);
			return p;
		}
	};

}
