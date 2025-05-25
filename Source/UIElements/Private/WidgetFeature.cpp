// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetFeature.h"
#include "flecs.h"
#include "ECS.h"
#include "UIFeature.h"
#include "TypographyFeature.h"
#include "ButtonFeature.h"
#include "Logging/StructuredLog.h"
#include <numeric>

namespace UIElements {
	void WidgetFeature::RegisterComponents(flecs::world& world) {
		using namespace ECS;
		world.component<Viewport>();

		world.component<Widget>().on_remove([](flecs::entity e, Widget& w) {w.Value.Reset(); });
		world.component<CompoundWidget>();

		world.component<Box>();
		world.component<HBox>();
		world.component<VBox>();

		world.component<Parented>().add(flecs::CanToggle);
		world.component<StyleSynced>().add(flecs::CanToggle);
	}

	void WidgetFeature::CreateSystems(flecs::world& world) {
		world.system("AddCompoundWidgetWidget")
			.without<Widget>()
			.with<CompoundWidget>()
			.each([](flecs::entity e) { e.set(Widget{ SNew(CompoundWidgetInstance) }); });

		world.system("AddBoxWidget")
			.without<Widget>()
			.with<Box>()
			.each([](flecs::entity e) { e.set(Widget{ SNew(SBox) }); });

		world.system("AddHorizontalBoxWidget")
			.without<Widget>()
			.with<HBox>()
			.each([](flecs::entity e) { e.set(Widget{ SNew(SHorizontalBox) }); });

		world.system("AddVerticalBoxWidget")
			.without<Widget>()
			.with<VBox>()
			.each([](flecs::entity e) { e.set(Widget{ SNew(SVerticalBox) }); });

		world.system("AddBorderWidget")
			.without<Widget>()
			.with<Border>()
			.each([](flecs::entity e) { e.set(Widget{ SNew(SBorder) }); });

		world.system("AddParented")
			.with<Widget>()
			.without<Parented>()
			.each([](flecs::entity e) { e.add<Parented>().disable<Parented>(); });

		world.system<const Widget>("ParentWidget")
			.with(flecs::ChildOf).second(flecs::Wildcard)
			.with<Parented>().id_flags(flecs::TOGGLE).without<Parented>()
			.each([](flecs::entity ch, const Widget& w) {
			flecs::entity parent = ch.parent();
			TSharedRef<SWidget> childWidget = w.Value.ToSharedRef();
			ch.enable<Parented>();
			if (parent.has<Viewport>())
			{
				GEngine->GameViewport->AddViewportWidgetContent(childWidget);
				return;
			}
			if (!parent.has<Widget>())
				return;
			TSharedRef<SWidget> parentWidget = parent.get_mut<Widget>()->Value.ToSharedRef();
			if (parent.has<CompoundWidget>())
			{
				StaticCastSharedRef<CompoundWidgetInstance>(parentWidget)
					->Slot().AttachWidget(childWidget);
				return;
			}
			if (parent.has<Box>()) {
				ParentToBox(childWidget, parent);
				return;
			}
			if (parent.has<HBox>()) {
				ParentToHorizontalBox(childWidget, parent);
				return;
			}
			if (parent.has<VBox>()) {
				ParentToVerticalBox(childWidget, parent);
				return;
			}
			if (parent.has<Border>() || parent.has<Button>()) {
				ParentToBorder(childWidget, parent);
			}
				});
	}
}
