// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetFeature.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
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

	void WidgetFeature::RegisterSystems(flecs::world& world) {}

	void WidgetFeature::Initialize(flecs::world& world) {
		//world.entity("textblock")
		//	.add<TextBlock>()
		//	.set<LocalizedText>({ "IT::Window" });
		//world.entity("widget").add<Widget>();

		world.system<Delay>()
			.each([](flecs::entity e, Delay& delay) {
			delay.RemainingTime -= e.world().delta_time();
			if (delay.RemainingTime <= 0.f) {
				if (delay.Callback) {
					delay.Callback();
				}
				e.remove<Delay>();
			}
		});

		flecs::entity myEntity = world.entity();

		FontFeature::AwaitDelay(myEntity, 3, [&world]() {
			UE_LOG(LogTemp, Warning, TEXT(">>> 3 seconds have passed!"));


			flecs::query<TextBlock> qTextBlocks = world.query<TextBlock>();
			flecs::query<Widget> qWidgets = world.query<Widget>();

			std::vector<flecs::entity> textBlocks;
			//qTextBlocks.each([&](flecs::entity e, TextBlock& tb) {
			//	tb.Value->SetText(FText::FromString("Hello, Dynamic World!"));
			//	textBlocks.push_back(e);
			//});

			qWidgets.each([&](flecs::entity e, Widget& widget) {
				for (auto& textBlockEntity : textBlocks) {
					if (textBlockEntity.is_valid() && widget.Value.IsValid()) {
						widget.Value->Slot()
							.AttachWidget(textBlockEntity.get_mut<TextBlock>()->Value.ToSharedRef());
					}
				}
				if (GEngine && GEngine->GameViewport) {
					GEngine->GameViewport->AddViewportWidgetContent(e.get<Widget>()->Value.ToSharedRef());
				}

				ecs_world_to_json_desc_t desc = { false, true };
				const char* jsonser = ecs_world_to_json(world, &desc);
				FString JsonString(jsonser);
				UE_LOGFMT(LogTemp, Warning, "Whole World >>> '{json}'", *JsonString);
			});
		});
	}
}
