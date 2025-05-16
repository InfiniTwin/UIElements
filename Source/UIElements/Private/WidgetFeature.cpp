// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetFeature.h"
#include "flecs.h"
#include "UIFeature.h"
#include "TextFeature.h"
#include "FontFeature.h"
#include "Logging/StructuredLog.h"

namespace UIElements {
	void WidgetFeature::RegisterComponents(flecs::world& world) {
		world.component<Widget>().on_remove([](flecs::entity e, Widget& w) {w.Value.Reset(); });

		world.component<Viewport>();

		world.component<CompoundWidget>();

		world.component<Border>();

		world.component<Attached>().add(flecs::CanToggle);
	}

	void WidgetFeature::CreateSystems(flecs::world& world) {
		world.system("AddWidgetToCompoundWidget")
			.with<CompoundWidget>()
			.without<Widget>()
			.each([](flecs::entity e) { e.set(Widget{ SNew(CompoundWidgetInstance) }); });

		world.system("AddWidgetToBorder")
			.with<Border>()
			.without<Widget>()
			.each([](flecs::entity e) { e.set(Widget{ SNew(SBorder) }); });

		world.system("MakeWidgetAttachable")
			.with<Widget>()
			.without<Attached>()
			.each([](flecs::entity e) { e.add<Attached>().disable<Attached>(); });

		world.system<const Widget>("AttachedWidgetToParent")
			.with(flecs::ChildOf)
			.second(flecs::Wildcard)
			.with<Attached>().id_flags(flecs::TOGGLE)
			.without<Attached>()
			.each([](flecs::entity child, const Widget& widget) {
			auto parent = child.parent();
			if (parent.has<Viewport>() && child.has<CompoundWidget>()) {
				child.enable<Attached>();
				GEngine->GameViewport->AddViewportWidgetContent(
					StaticCastSharedPtr<CompoundWidgetInstance>(widget.Value).ToSharedRef());
				return;
			}
			if (parent.has<CompoundWidget>() && parent.has<Widget>()) {
				child.enable<Attached>();
				StaticCastSharedPtr<CompoundWidgetInstance>(parent.get_mut<Widget>()->Value)->Slot()
					.AttachWidget(widget.Value.ToSharedRef());
			}
				});
	}

	void WidgetFeature::Initialize(flecs::world& world) {
		world.entity(GameViewport).add<Viewport>();

		world.system<Delay>()
			.each([](flecs::entity e, Delay& delay) {
			delay.RemainingTime -= e.world().delta_time();
			if (delay.RemainingTime <= 0.f) {
				if (delay.Callback)
					delay.Callback();
				e.remove<Delay>();
			}
				});

		flecs::entity myEntity = world.entity();
		FontFeature::AwaitDelay(myEntity, 3, [&world]() {
			world.set<Locale>({ "de" });
			});

		flecs::entity myEntity1 = world.entity();
		FontFeature::AwaitDelay(myEntity1, 6, [&world]() {
			world.set<Locale>({ "it" });

			ecs_world_to_json_desc_t desc = { false, true };
			const char* jsonser = ecs_world_to_json(world, &desc);
			FString JsonString(jsonser);
			//UE_LOGFMT(LogTemp, Warning, "Whole World >>> '{json}'", *JsonString);
			});
	}
}
