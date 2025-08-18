// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetFeature.h"
#include "flecs.h"
#include "ECS.h"
#include "TypographyFeature.h"
#include "ButtonFeature.h"
#include "WindowFeature.h"
#include "ActionFeature.h"
#include "Engine/UserInterfaceSettings.h"

namespace UI {
	void WidgetFeature::RegisterComponents(flecs::world& world) {
		using namespace ECS;
		world.component<SlateApplication>().add(flecs::OnInstantiate, flecs::Inherit);
		world.component<Viewport>();

		world.component<UIScale>().member<float>(VALUE).add(flecs::Singleton);

		world.component<UIOf>();

		world.component<Widget>().add(flecs::OnInstantiate, flecs::Inherit);
		world.component<WidgetInstance>().on_remove([](flecs::entity entity, WidgetInstance& widget) { widget.Value.Reset(); });

		world.component<ConstraintCanvas>();

		world.component<Collection>().add(flecs::OnInstantiate, flecs::Inherit);

		world.component<Border>().add(flecs::OnInstantiate, flecs::Inherit);

		world.component<Menu>().add(flecs::OnInstantiate, flecs::Inherit);
		world.component<MenuPlacement>().member<int>(VALUE).add(flecs::OnInstantiate, flecs::Inherit);
		world.component<MenuContent>().add(flecs::OnInstantiate, flecs::Inherit);

		world.component<Box>().add(flecs::OnInstantiate, flecs::Inherit);
		world.component<HBox>().add(flecs::OnInstantiate, flecs::Inherit);
		world.component<VBox>().add(flecs::OnInstantiate, flecs::Inherit);
		world.component<ScrollBox>().add(flecs::OnInstantiate, flecs::Inherit);

		world.component<FillHeight>().member<float>(VALUE).add(flecs::OnInstantiate, flecs::Inherit);
		world.component<FillWidth>().member<float>(VALUE).add(flecs::OnInstantiate, flecs::Inherit);

		world.component<VAlign>().member<int>(VALUE).add(flecs::OnInstantiate, flecs::Inherit);
		world.component<HAlign>().member<int>(VALUE).add(flecs::OnInstantiate, flecs::Inherit);

		world.component<Orientation>().member<int>(VALUE).add(flecs::OnInstantiate, flecs::Inherit);

		world.component<Padding>().member<FMargin>(VALUE).add(flecs::OnInstantiate, flecs::Inherit);

		world.component<WidgetState>().add(flecs::Exclusive);
		world.component<Visibility>().add(flecs::Exclusive);

		world.component<Attached>().add(flecs::CanToggle);
		world.component<Order>().member<int>(VALUE).add(flecs::OnInstantiate, flecs::Inherit);
	}

	void WidgetFeature::CreateObservers(flecs::world& world) {
		world.observer<const UIScale>("UpdateApplicationUIScale")
			.event(flecs::OnSet)
			.each([](const UIScale& scale) {
			FSlateApplication::Get().SetApplicationScale(scale.Value);
			GetMutableDefault<UUserInterfaceSettings>(UUserInterfaceSettings::StaticClass())
				->ApplicationScale = scale.Value;
				});

		world.observer<>("AddWidgetInstance")
			.with<Widget>()
			.event(flecs::OnAdd)
			.each([](flecs::entity widget) {
			if (widget.has<Border>())
				widget.set(WidgetInstance{ SNew(SBorder) });
			else if (widget.has<ConstraintCanvas>())
				widget.set(WidgetInstance{ SNew(SConstraintCanvas) });
			else if (widget.has<Box>())
				widget.set(WidgetInstance{ SNew(SBox).Padding(0).WidthOverride(FOptionalSize()).HeightOverride(FOptionalSize()) });
			else if (widget.has<HBox>())
				widget.set(WidgetInstance{ SNew(SHorizontalBox) });
			else if (widget.has<VBox>())
				widget.set(WidgetInstance{ SNew(SVerticalBox) });
			else if (widget.has<ScrollBox>())
				widget.set(WidgetInstance{ SNew(SScrollBox).Orientation(static_cast<EOrientation>(widget.try_get<Orientation>()->Value)) });
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

		world.observer<>("SetWidgetVisibility")
			.with<Visibility>(flecs::Wildcard)
			.with<WidgetInstance>()
			.event(flecs::OnAdd)
			.each([](flecs::entity entity) {
			EVisibility widgetVisibility = EVisibility::Visible;
			Visibility visibility = entity.target<Visibility>().to_constant<Visibility>();

			switch (visibility) {
			case Visible:               widgetVisibility = EVisibility::Visible; break;
			case Collapsed:             widgetVisibility = EVisibility::Collapsed; break;
			case Hidden:                widgetVisibility = EVisibility::Hidden; break;
			case HitTestInvisible:      widgetVisibility = EVisibility::HitTestInvisible; break;
			case SelfHitTestInvisible:  widgetVisibility = EVisibility::SelfHitTestInvisible; break;
			case All:                   widgetVisibility = EVisibility::All; break;
			}

			entity.try_get_mut<WidgetInstance>()->Value.ToSharedRef()
				->SetVisibility(widgetVisibility);
				});

		world.observer<>("SwitchMenu")
			.with<Menu>()
			.with<WidgetState>().second(flecs::Wildcard)
			.event(flecs::OnSet)
			.each([](flecs::entity entity) {
			StaticCastSharedRef<SMenuAnchor>(entity.try_get<WidgetInstance>()->Value.ToSharedRef())
				->SetIsOpen(entity.has(Opened));
				});

		world.observer<>("TriggerWidgetAction")
			.with<WidgetState>().second(flecs::Wildcard)
			.or_()
			.with<Visibility>().second(flecs::Wildcard)
			.or_()
			.with<ButtonState>().second(flecs::Wildcard)
			.or_()
			.with<CheckBoxState>().second(flecs::Wildcard)
			.event(flecs::OnSet)
			.each([](flecs::iter& it, size_t i) {
			auto event = it.pair(0);
			it.entity(i).children([&event](flecs::entity action) {
				if (action.has<ECS::Action>() && action.has(event))
					action.enable<ECS::Action>(); });
				});

		world.observer<>("TriggerWidgetInverseAction")
			.with<WidgetState>().second(flecs::Wildcard)
			.or_()
			.with<Visibility>().second(flecs::Wildcard)
			.or_()
			.with<ButtonState>().second(flecs::Wildcard)
			.or_()
			.with<CheckBoxState>().second(flecs::Wildcard)
			.event(flecs::OnRemove)
			.each([](flecs::iter& it, size_t t) {
			auto event = it.pair(0);
			it.entity(t).children([&event](flecs::entity action) {
				if (action.has<ECS::Invert>() && action.has(event))
					action.enable<ECS::Invert>(); });
				});

		world.observer<WidgetInstance>("DetachWidget")
			.event(flecs::OnRemove)
			.each([](flecs::entity entity, WidgetInstance& instance) {
			TSharedPtr<SWidget> widget = instance.Value;

			if (entity.has<HBox>() || entity.has<VBox>())
				StaticCastSharedPtr<SBoxPanel>(widget)->ClearChildren();
			else if (entity.has<ScrollBox>())
				StaticCastSharedPtr<SScrollBox>(widget)->ClearChildren();
			else if (entity.has<ConstraintCanvas>())
				StaticCastSharedPtr<SConstraintCanvas>(widget)->ClearChildren();
			else if (entity.has<Border>() || entity.has<Button>())
				StaticCastSharedPtr<SBorder>(widget)->ClearContent();
			else
				return;

			widget->Invalidate(EInvalidateWidgetReason::Layout | EInvalidateWidgetReason::Paint);
			instance.Value.Reset();
				});
	}

	void WidgetFeature::CreateQueries(flecs::world& world) {
		world.set(QueryUIOf{
			world.query_builder<>(COMPONENT(QueryUIOf))
			.with<UIOf>().second("$source")
			.cached().build() });
	};

	void WidgetFeature::CreateSystems(flecs::world& world) {
		world.system<const WidgetInstance, const Order>("AttachWidget")
			.with(flecs::ChildOf).second(flecs::Wildcard)
			.order_by(SortOrder)
			.with<Attached>().id_flags(flecs::TOGGLE).without<Attached>()
			.each([](flecs::entity child, const WidgetInstance& widget, Order) {
			flecs::entity parent = child.parent();
			child.enable<Attached>();

			if (child.has<Window>()) return;

			if (parent.has<Viewport>()) {
				GEngine->GameViewport->AddViewportWidgetContent(widget.Value.ToSharedRef());
				return;
			}

			if (!parent.has<WidgetInstance>())
				return;
			TSharedRef<SWidget> parentWidget = parent.try_get_mut<WidgetInstance>()->Value.ToSharedRef();

			if (parent.has<Border>())
				SetContent<SBorder>(parentWidget, child);
			else if (parent.has<ConstraintCanvas>())
				AttachToConstraintCanvas(parentWidget, child);
			else if (parent.has<Box>())
				SetContent<SBox>(parentWidget, child);
			else if (parent.has<HBox>())
				AttachToHorizontalBox(parentWidget, child);
			else if (parent.has<VBox>())
				AttachToVerticalBox(parentWidget, child);
			else if (parent.has<ScrollBox>())
				AttachToScrollBox(parentWidget, child);
			else if (parent.has<Button>())
				SetContent<SButton>(parentWidget, child);
			else if (parent.has<CheckBox>())
				SetContent<SCheckBox>(parentWidget, child);
			else if (parent.has<Menu>() && !child.has<MenuContent>())
				SetContent<SMenuAnchor>(parentWidget, child);
			else if (parent.has<Window>())
				SetContent<SWindow>(parentWidget, child);
				});
	}
}
