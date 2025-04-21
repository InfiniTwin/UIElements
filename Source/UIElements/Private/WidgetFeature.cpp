// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetFeature.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "flecs.h"
#include "UIFeature.h"
#include "TextFeature.h"
#include "FontFeature.h"
#include "Logging/StructuredLog.h"

namespace UIElements {
	void WidgetFeature::RegisterComponents(flecs::world& world) {
		world.component<Widget>()
			.on_add([](flecs::entity e, Widget& w) { SAssignNew(w.Value, CompoundWidget); })
			.on_remove([](flecs::entity e, Widget& w) {w.Value.Reset(); });
	}

	void WidgetFeature::RegisterObservers(flecs::world& world) {
		world.observer<>()
			.event(flecs::OnSet)
			.each([&world](const LocalizedText& localizedText, const TextBlock& textBlock) {
				});
	}

	void WidgetFeature::Initialize(flecs::world& world) {
		//world.entity("textblock")
		//	.add<TextBlock>()
		//	.set<LocalizedText>({ "IT::Window" });
		//world.entity("widget").add<Widget>();

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
			flecs::query<TextBlock> qTextBlocks = world.query<TextBlock>();
			flecs::query<Widget> qWidgets = world.query<Widget>();

			std::vector<flecs::entity> textBlocks;
			qTextBlocks.each([&](flecs::entity e, TextBlock& tb) {
				textBlocks.push_back(e);
				});

			qWidgets.each([&](flecs::entity e, Widget& widget) {
				for (auto& textBlockEntity : textBlocks)
					if (textBlockEntity.is_valid() && widget.Value.IsValid()) {
						widget.Value->Slot()
							.AttachWidget(textBlockEntity.get_mut<TextBlock>()->Value.ToSharedRef());
						//textBlockEntity.child_of(e);
					}
				if (GEngine && GEngine->GameViewport)
					GEngine->GameViewport->AddViewportWidgetContent(e.get<Widget>()->Value.ToSharedRef());
				});
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
