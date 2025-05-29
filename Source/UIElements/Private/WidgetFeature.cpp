// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetFeature.h"
#include "flecs.h"
#include "ECS.h"
#include "UIFeature.h"
#include "TypographyFeature.h"
#include "ButtonFeature.h"

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
	}

	void WidgetFeature::CreateObservers(flecs::world& world) {
		world.observer<UIWidget>("AddWidget")
			.event(flecs::OnAdd)
			.each([](flecs::entity e, UIWidget) {
			if (e.has<CompoundWidget>())
				e.set(Widget{ SNew(CompoundWidgetInstance) });
			else if (e.has<Box>())
				e.set(Widget{ SNew(SBox) });
			else if (e.has<HBox>())
				e.set(Widget{ SNew(SHorizontalBox) });
			else if (e.has<VBox>())
				e.set(Widget{ SNew(SVerticalBox) });
			else if (e.has<Border>())
				e.set(Widget{ SNew(SBorder) });
			else if (e.has<Button>())
				e.set(Widget{ SNew(SButton) });			
			else if (e.has<TextBlock>())
				AddTextBlockWidget(e);

			e.add<Parented>().disable<Parented>();
				});
	}


	void WidgetFeature::CreateSystems(flecs::world& world) {
		world.system<const Widget>("ParentWidget")
			.with(flecs::ChildOf).second(flecs::Wildcard)
			.with<Parented>().id_flags(flecs::TOGGLE).without<Parented>()
			.each([](flecs::entity child, const Widget& w) {
			flecs::entity parent = child.parent();
			TSharedRef<SWidget> childWidget = w.Value.ToSharedRef();
			child.enable<Parented>();
			auto cp = child.path();
			auto pp = parent.path();
			if (parent.has<Viewport>())
			{
				GEngine->GameViewport->AddViewportWidgetContent(childWidget);
				return;
			}

			if (!parent.has<Widget>())
				return;
			TSharedRef<SWidget> parentWidget = parent.get_mut<Widget>()->Value.ToSharedRef();

			if (parent.has<CompoundWidget>())
				ParentToCompoundWidget(childWidget, parentWidget);
			else if (parent.has<Box>())
				ParentToBox(childWidget, parentWidget);
			else if (parent.has<HBox>())
				ParentToHorizontalBox(child, parentWidget);
			else if (parent.has<VBox>())
				ParentToVerticalBox(child, parentWidget);
			else if (parent.has<Border>() || parent.has<Button>())
				ParentToBorder(childWidget, parentWidget);
				});
	}
}
