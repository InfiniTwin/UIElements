// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetFeature.h"
#include "flecs.h"
#include "ECS.h"
#include "OpaqueTypes.h"
#include "UIFeature.h"
#include "TypographyFeature.h"
#include "ButtonFeature.h"
#include "ToggleFeature.h"

namespace UIElements {
	void WidgetFeature::RegisterOpaqueTypes(flecs::world& world) {
		// EHorizontalAlignment <=> int
		world.component<EHorizontalAlignment>()
			.opaque(flecs::I32)
			.serialize([](const flecs::serializer* s, const EHorizontalAlignment* data)
				{
					int value = static_cast<int>(*data);
					return s->value(flecs::I32, &value);
				})
			.assign_int([](EHorizontalAlignment* data, int64_t value)
				{
					if (value >= static_cast<int64_t>(HAlign_Fill) &&
						value <= static_cast<int64_t>(HAlign_Right)) {
						*data = static_cast<EHorizontalAlignment>(value);
					}
					else {
						*data = HAlign_Fill;
					}
				});

		// EVerticalAlignment <=> int
		world.component<EVerticalAlignment>()
			.opaque(flecs::I32)
			.serialize([](const flecs::serializer* s, const EVerticalAlignment* data)
				{
					int value = static_cast<int>(*data);
					return s->value(flecs::I32, &value);
				})
			.assign_int([](EVerticalAlignment* data, int64_t value)
				{
					if (value >= static_cast<int64_t>(VAlign_Fill) &&
						value <= static_cast<int64_t>(VAlign_Bottom)) {
						*data = static_cast<EVerticalAlignment>(value);
					}
					else {
						*data = VAlign_Fill;
					}
				});
	}

	void WidgetFeature::RegisterComponents(flecs::world& world) {
		using namespace ECS;
		world.component<Viewport>();

		world.component<UIWidget>().add(flecs::OnInstantiate, flecs::Inherit);
		world.component<Widget>().on_remove([](flecs::entity e, Widget& w) {w.Value.Reset(); });

		world.component<CompoundWidget>();

		world.component<Parented>().add(flecs::CanToggle);

		world.component<Box>().add(flecs::OnInstantiate, flecs::Inherit);
		world.component<HBox>().add(flecs::OnInstantiate, flecs::Inherit);
		world.component<VBox>().add(flecs::OnInstantiate, flecs::Inherit);

		world.component<HAlign>().member<EHorizontalAlignment>(VALUE).add(flecs::OnInstantiate, flecs::Inherit);
		world.component<VAlign>().member<EVerticalAlignment>(VALUE).add(flecs::OnInstantiate, flecs::Inherit);
		world.component<Padding>().member<std::vector<float>>(VALUE).add(flecs::OnInstantiate, flecs::Inherit);

		world.component<StyleSynced>().add(flecs::CanToggle);

		world.component<Order>().member<int>(VALUE).add(flecs::OnInstantiate, flecs::Inherit);
	}

	void WidgetFeature::CreateObservers(flecs::world& world) {
		world.observer<>("AddWidget")
			.with<UIWidget>()
			.event(flecs::OnAdd)
			.each([](flecs::entity entity) {
			if (entity.has<CompoundWidget>())
				entity.set(Widget{ SNew(CompoundWidgetInstance) });
			else if (entity.has<Box>())
				entity.set(Widget{ SNew(SBox) });
			else if (entity.has<HBox>())
				entity.set(Widget{ SNew(SHorizontalBox) });
			else if (entity.has<VBox>())
				entity.set(Widget{ SNew(SVerticalBox) });
			else if (entity.has<Border>())
				entity.set(Widget{ SNew(SBorder) });
			else if (entity.has<Button>())
				AddButtonWidget(entity);
			else if (entity.has<Toggle>())
				entity.set(Widget{ SNew(SCheckBox) });
			else if (entity.has<TextBlock>())
				AddTextBlockWidget(entity);

			entity.add<Parented>().disable<Parented>();
				});
	}

	void WidgetFeature::CreateSystems(flecs::world& world) {
		world.system<const Widget, const Order>("ParentWidget")
			.with(flecs::ChildOf).second(flecs::Wildcard)
			.order_by(SortOrder)
			.with<Parented>().id_flags(flecs::TOGGLE).without<Parented>()
			.each([](flecs::entity child, const Widget& w, Order) {
			flecs::entity parent = child.parent();
			child.enable<Parented>();
			if (parent.has<Viewport>())
			{
				GEngine->GameViewport->AddViewportWidgetContent(w.Value.ToSharedRef());
				return;
			}

			if (!parent.has<Widget>())
				return;
			TSharedRef<SWidget> parentWidget = parent.get_mut<Widget>()->Value.ToSharedRef();

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
