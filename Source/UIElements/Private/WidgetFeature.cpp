// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetFeature.h"
#include "flecs.h"
#include "ECS.h"
#include "UIFeature.h"
#include "TypographyFeature.h"
#include "ButtonFeature.h"
#include "WindowFeature.h"

namespace UI {
	void WidgetFeature::RegisterComponents(flecs::world& world) {
		using namespace ECS;
		world.component<SlateApplication>().add(flecs::OnInstantiate, flecs::Inherit);
		world.component<Viewport>();

		world.component<Widget>().add(flecs::OnInstantiate, flecs::Inherit);
		world.component<WidgetInstance>();

		world.component<Border>().add(flecs::OnInstantiate, flecs::Inherit);

		world.component<ConstraintCanvas>();
		world.component<CompoundWidget>();

		world.component<Menu>().add(flecs::OnInstantiate, flecs::Inherit);
		world.component<MenuPlacement>().member<int>(VALUE).add(flecs::OnInstantiate, flecs::Inherit);
		world.component<MenuContent>().add(flecs::OnInstantiate, flecs::Inherit);

		world.component<Box>().add(flecs::OnInstantiate, flecs::Inherit);
		world.component<HBox>().add(flecs::OnInstantiate, flecs::Inherit);
		world.component<VBox>().add(flecs::OnInstantiate, flecs::Inherit);

		world.component<FillHeight>().member<float>(VALUE).add(flecs::OnInstantiate, flecs::Inherit);
		world.component<FillWidth>().member<float>(VALUE).add(flecs::OnInstantiate, flecs::Inherit);

		world.component<VAlign>().member<int>(VALUE).add(flecs::OnInstantiate, flecs::Inherit);
		world.component<HAlign>().member<int>(VALUE).add(flecs::OnInstantiate, flecs::Inherit);

		world.component<Padding>().member<FMargin>(VALUE).add(flecs::OnInstantiate, flecs::Inherit);

		world.component<Attached>().add(flecs::CanToggle);
		world.component<Order>().member<int>(VALUE).add(flecs::OnInstantiate, flecs::Inherit);
	}

	void WidgetFeature::CreateObservers(flecs::world& world) {
		world.observer<>("AddWidgetInstance")
			.with<Widget>()
			.event(flecs::OnAdd)
			.each([&world](flecs::entity widget) {
			if (widget.has<Border>())
				widget.set(WidgetInstance{ SNew(SBorder) });
			else if (widget.has<ConstraintCanvas>())
				widget.set(WidgetInstance{ SNew(SConstraintCanvas) });
			else if (widget.has<CompoundWidget>())
				widget.set(WidgetInstance{ SNew(CompoundWidgetElement) });
			else if (widget.has<Box>())
				widget.set(WidgetInstance{ SNew(SBox).Padding(0).WidthOverride(FOptionalSize()).HeightOverride(FOptionalSize()) });
			else if (widget.has<HBox>())
				widget.set(WidgetInstance{ SNew(SHorizontalBox) });
			else if (widget.has<VBox>())
				widget.set(WidgetInstance{ SNew(SVerticalBox) });
			else if (widget.has<Button>())
				AddButtonWidget(widget);
			else if (widget.has<CheckBox>())
				AddCheckBoxWidget(widget);
			else if (widget.has<TextBlock>())
				AddTextBlockWidget(widget);
			else if (widget.has<Menu>())
				AddMenuWidget(widget);

			widget.add<Attached>().disable<Attached>();
				});

		world.observer<>("SwitchMenu")
			.with<Menu>()
			.with<WidgetState>().second(flecs::Wildcard)
			.event(flecs::OnSet)
			.each([&world](flecs::entity entity) {
			StaticCastSharedRef<SMenuAnchor>(entity.try_get<WidgetInstance>()->Value.ToSharedRef())
				->SetIsOpen(entity.has(WidgetState::Opened));
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

			if (child.has<Window>()) return;

			if (parent.has<Viewport>()) {
				GEngine->GameViewport->AddViewportWidgetContent(w.Value.ToSharedRef());
				return;
			}

			if (!parent.has<WidgetInstance>())
				return;
			TSharedRef<SWidget> parentWidget = parent.try_get_mut<WidgetInstance>()->Value.ToSharedRef();

			if (parent.has<Border>())
				SetContent<SBorder>(parentWidget, child);
			else if (parent.has<ConstraintCanvas>())
				AttachToConstraintCanvas(parentWidget, child);
			else if (parent.has<CompoundWidget>())
				AttachToCompoundWidget(parentWidget, w.Value.ToSharedRef());
			else if (parent.has<Box>())
				SetContent<SBox>(parentWidget, child);
			else if (parent.has<HBox>())
				AttachToHorizontalBox(parentWidget, child);
			else if (parent.has<VBox>())
				AttachToVerticalBox(parentWidget, child);
			else if (parent.has<Button>())
				SetContent<SButton>(parentWidget, child);
			else if (parent.has<CheckBox>())
				SetContent<SCheckBox>(parentWidget, child);
			else if (parent.has<Menu>() && !child.has<MenuContent>())
				SetContent<SMenuAnchor>(parentWidget, child);
				});
	}
}
