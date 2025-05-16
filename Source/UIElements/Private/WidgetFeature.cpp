// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetFeature.h"
#include "flecs.h"
#include "UIFeature.h"
#include "TextFeature.h"
#include "FontFeature.h"
#include "Logging/StructuredLog.h"

namespace UIElements {
	void WidgetFeature::RegisterComponents(flecs::world& world) {
		world.component<Attached>().add(flecs::CanToggle);

		world.component<Viewport>();

		world.component<Widget>()
			.on_remove([](flecs::entity e, Widget& w) {w.Value.Reset(); });

		world.component<CompoundWidget>();

		world.component<Border>();
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
			.each([](flecs::entity e) { e.add<Attached>(); e.disable<Attached>(); });

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


		auto widget = world.prefab("WidgetPREFAB")
			.set_auto_override(Widget{});

		//auto icon = world.prefab(COMPONENT(Icon))
		//	.set<IconFont>({ FSlateFontInfo() })
		//	.set_auto_override(Icon{ "??" });

		auto localizedText = world.prefab("LocalizedTextPREFAB")
			.set_auto_override(LocalizedText{});

		//auto labelSmall = world.prefab(COMPONENT(LabelSmall))
		//	.set<LabelSmall>({ FSlateFontInfo("font400", 10) });

		auto textBlock = world.prefab("TextBlockPREFAB")
			.is_a(widget)
			.is_a(localizedText)
			.add<TextBlock>();

		//auto assistChip = world.prefab()

		//auto windowToggle = world.entity("WindowToggle PREFABED")

		FString JsonStr(textBlock.to_json().c_str());

		//UE_LOGFMT(LogTemp, Warning, "PREFAB >>> '{json}'", *JsonStr);

		//FString JsonStr(world.get_scope().to_json().c_str());
		UE_LOGFMT(LogTemp, Warning, "PREFAB >>> '{json}'", *JsonStr);
	}
}
