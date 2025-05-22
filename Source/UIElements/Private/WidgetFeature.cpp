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

		world.component<ParentSynced>().add(flecs::CanToggle);
		world.component<StyleSynced>().add(flecs::CanToggle);
	}

	void WidgetFeature::CreateSystems(flecs::world& world) {
		world.system("AddCompoundWidgetWidget")
			.with<CompoundWidget>()
			.without<Widget>()
			.each([](flecs::entity e) { e.set(Widget{ SNew(CompoundWidgetInstance) }); });

		world.system("AddBoxWidget")
			.with<Box>()
			.without<Widget>()
			.each([](flecs::entity e) { e.set(Widget{ SNew(SBox) }); });

		world.system("AddHorizontalBoxWidget")
			.with<HBox>()
			.without<Widget>()
			.each([](flecs::entity e) { e.set(Widget{ SNew(SHorizontalBox) }); });

		world.system("AddVerticalBoxWidget")
			.with<VBox>()
			.without<Widget>()
			.each([](flecs::entity e) { e.set(Widget{ SNew(SVerticalBox) }); });

		world.system("AddBorderWidget")
			.with<Border>()
			.without<Widget>()
			.each([](flecs::entity e) { e.set(Widget{ SNew(SBorder) }); });

		world.system("AddParentSynced")
			.with<Widget>()
			.without<ParentSynced>()
			.each([](flecs::entity e) { e.add<ParentSynced>().disable<ParentSynced>(); });

		world.system<const Widget>("SyncParent")
			.with(flecs::ChildOf).second(flecs::Wildcard)
			.with<ParentSynced>().id_flags(flecs::TOGGLE).without<ParentSynced>()
			.each([](flecs::entity ch, const Widget& w) {
			flecs::entity parent = ch.parent();
			TSharedRef<SWidget> childWidget = w.Value.ToSharedRef();
			ch.enable<ParentSynced>();
			if (parent.has<Viewport>())
			{
				GEngine->GameViewport->AddViewportWidgetContent(childWidget);
				return;
			}
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
