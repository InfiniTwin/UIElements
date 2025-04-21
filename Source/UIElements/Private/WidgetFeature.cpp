// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetFeature.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "flecs.h"
#include "ECS.h"
#include "UIFeature.h"
#include "TextFeature.h"
#include "FontFeature.h"
#include "Logging/StructuredLog.h"

namespace UIElements {
	void WidgetFeature::RegisterComponents(flecs::world& world) {
		world.component<Viewport>();

		world.component<CompoundWidget>()
			.on_add([](flecs::entity e, CompoundWidget& cw) { SAssignNew(cw.Value, CompoundWidgetInstance); })
			.on_remove([](flecs::entity e, CompoundWidget& cw) {cw.Value.Reset(); });

		world.component<Widget>()
			.on_remove([](flecs::entity e, Widget& w) {w.Value.Reset(); });
	}

	void WidgetFeature::RegisterObservers(flecs::world& world) {
		// Attach child SCompoundWidget to parent GameViewport
		world.observer()
			.with<CompoundWidget>()
			.with(flecs::ChildOf)
			.second(flecs::Wildcard)
			.event(flecs::OnAdd)
			.each([](flecs::iter& it, size_t i) {
			auto parent = it.pair(1).second();
			if (parent.has<Viewport>())
				GEngine->GameViewport->AddViewportWidgetContent(it.entity(i).get_mut<CompoundWidget>()->Value.ToSharedRef());
				});

		// Attach child SWidget to parent SCompoundWidget
		world.observer()
			.with<Widget>()
			.with(flecs::ChildOf)
			.second(flecs::Wildcard)
			.event(flecs::OnAdd)
			.each([](flecs::iter& it, size_t i) {
			auto parent = it.pair(1).second();
			if (parent.has<CompoundWidget>())
				it.pair(1).second().get_mut<CompoundWidget>()->Value->Slot()
				.AttachWidget(it.entity(i).get<Widget>()->Value.ToSharedRef());
				});
	}

	void WidgetFeature::Initialize(flecs::world& world) {
		world.entity("GameViewport").add<Viewport>();

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
			UE_LOGFMT(LogTemp, Warning, "Whole World >>> '{json}'", *JsonString);
			});
	}
}
