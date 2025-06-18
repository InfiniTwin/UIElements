// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetFeature.h"
#include "flecs.h"
#include "ECS.h"
#include "UIFeature.h"
#include "TypographyFeature.h"
#include "ButtonFeature.h"

namespace UI {
	void WidgetFeature::RegisterComponents(flecs::world& world) {
		using namespace ECS;
		world.component<Viewport>();

		world.component<Widget>().add(flecs::OnInstantiate, flecs::Inherit);
		world.component<WidgetInstance>()
			.on_remove([](flecs::entity e, WidgetInstance& w) {w.Value.Reset(); });

		world.component<CompoundWidget>();

		world.component<Attached>().add(flecs::CanToggle);

		world.component<Box>().add(flecs::OnInstantiate, flecs::Inherit);
		world.component<HBox>().add(flecs::OnInstantiate, flecs::Inherit);
		world.component<VBox>().add(flecs::OnInstantiate, flecs::Inherit);

		world.component<HAlign>().member<int>(VALUE).add(flecs::OnInstantiate, flecs::Inherit);
		world.component<VAlign>().member<int>(VALUE).add(flecs::OnInstantiate, flecs::Inherit);
		world.component<Padding>()
			.member<float>(MEMBER(Padding::Left))
			.member<float>(MEMBER(Padding::Topp))
			.member<float>(MEMBER(Padding::Right))
			.member<float>(MEMBER(Padding::Bottom))
			.add(flecs::OnInstantiate, flecs::Inherit);

		world.component<StyleSynced>().add(flecs::CanToggle);

		world.component<Order>().member<int>(VALUE).add(flecs::OnInstantiate, flecs::Inherit);
	}

	void WidgetFeature::CreateObservers(flecs::world& world) {
		world.observer<>("AddWidgetInstance")
			.with<Widget>()
			.event(flecs::OnAdd)
			.each([&world](flecs::entity entity) {
			if (entity.has<CompoundWidget>())
				entity.set(WidgetInstance{ SNew(CompoundWidgetInstance) });
			else if (entity.has<Box>())
				entity.set(WidgetInstance{ SNew(SBox).Padding(0)});
			else if (entity.has<HBox>())
				entity.set(WidgetInstance{ SNew(SHorizontalBox) });
			else if (entity.has<VBox>())
				entity.set(WidgetInstance{ SNew(SVerticalBox) });
			else if (entity.has<Button>())
				AddButtonWidget(world, entity);
			else if (entity.has<Border>())
				AddBorderWidget(entity);
			else if (entity.has<Toggle>())
				entity.set(WidgetInstance{ SNew(SCheckBox) });
			else if (entity.has<TextBlock>())
				AddTextBlockWidget(entity);

			entity.add<Attached>().disable<Attached>();
				});
	}

	void WidgetFeature::CreateSystems(flecs::world& world) {
		world.system<const WidgetInstance, const Order>("AttachWidget")
			.with(flecs::ChildOf).second(flecs::Wildcard)
			.order_by(SortOrder)
			.with<Attached>().id_flags(flecs::TOGGLE).without<Attached>()
			.each([](flecs::entity child, const WidgetInstance& w, Order) {
			flecs::entity parent = child.parent();
			child.enable<Attached>();
			if (parent.has<Viewport>()) {
				GEngine->GameViewport->AddViewportWidgetContent(w.Value.ToSharedRef());
				return;
			}

			if (!parent.has<WidgetInstance>())
				return;
			TSharedRef<SWidget> parentWidget = parent.get_mut<WidgetInstance>()->Value.ToSharedRef();

			if (parent.has<CompoundWidget>())
				AttachToCompoundWidget(w.Value.ToSharedRef(), parentWidget);
			else if (parent.has<Box>())
				AttachToBox(child, parentWidget);
			else if (parent.has<HBox>())
				AttachToHorizontalBox(child, parentWidget);
			else if (parent.has<VBox>())
				AttachToVerticalBox(child, parentWidget);
			else if (parent.has<Border>() || parent.has<Button>())
				AttachToBorder(child, parentWidget);
			else if (parent.has<Toggle>())
				AttachToCheckBox(child, parentWidget);
				});
	}
}
