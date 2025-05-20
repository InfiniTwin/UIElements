// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetFeature.h"
#include "flecs.h"
#include "UIFeature.h"
#include "TypographyFeature.h"
#include "ButtonFeature.h"
#include "Logging/StructuredLog.h"

namespace UIElements {
	void WidgetFeature::RegisterComponents(flecs::world& world) {
		world.component<Widget>().on_remove([](flecs::entity e, Widget& w) {w.Value.Reset(); });

		world.component<Viewport>();

		world.component<CompoundWidget>();

		world.component<Border>();

		world.component<Attached>().add(flecs::CanToggle);
		world.component<StyleIsSet>().add(flecs::CanToggle);
	}

	void WidgetFeature::CreateSystems(flecs::world& world) {
		world.system("AddCompoundWidgetWidget")
			.with<CompoundWidget>()
			.without<Widget>()
			.each([](flecs::entity e) { e.set(Widget{ SNew(CompoundWidgetInstance) }); });

		world.system("AddBorderWidget")
			.with<Border>()
			.without<Widget>()
			.each([](flecs::entity e) { e.set(Widget{ SNew(SBorder) }); });

		world.system("AddAttached")
			.with<Widget>()
			.without<Attached>()
			.each([](flecs::entity e) { e.add<Attached>().disable<Attached>(); });

		world.system<const Widget>("ParentWidget")
			.with(flecs::ChildOf).second(flecs::Wildcard)
			.with<Attached>().id_flags(flecs::TOGGLE).without<Attached>()
			.each([](flecs::entity child, const Widget& widget) {
			auto parent = child.parent();
			child.enable<Attached>();
			if (parent.has<Viewport>())
			{
				GEngine->GameViewport->AddViewportWidgetContent(widget.Value.ToSharedRef());
				return;
			}
			if (parent.has<CompoundWidget>())
			{
				StaticCastSharedPtr<CompoundWidgetInstance>(parent.get_mut<Widget>()->Value)->Slot()
					.AttachWidget(widget.Value.ToSharedRef());
				return;
			}
			if (parent.has<Border>() || parent.has<Button>())
				StaticCastSharedPtr<SBorder>(parent.get_mut<Widget>()->Value)->SetContent(widget.Value.ToSharedRef());
				});
	}
}
