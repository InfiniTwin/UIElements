// Fill out your copyright notice in the Description page of Project Settings.

#include "UIFeature.h"
#include "flecs.h"
#include "ECS.h"
#include "ActionFeature.h"
#include "UIElements.h"
#include "Engine/UserInterfaceSettings.h"

namespace UI {
	void UIFeature::RegisterComponents(flecs::world& world) {
		using namespace ECS;
		world.component<UIScale>().member<float>(VALUE);

		world.component<Event>().add(flecs::Exclusive);
	}

	void UIFeature::CreateObservers(flecs::world& world) {
		world.observer<const UIScale>("UpdateApplicationUIScale")
			.term_at(0).singleton()
			.event(flecs::OnSet)
			.each([&world](const UIScale& scale) {
			GetMutableDefault<UUserInterfaceSettings>(UUserInterfaceSettings::StaticClass())
				->ApplicationScale = scale.Value; });

		world.observer<>("TriggerUIAction")
			.with<Event>().second(flecs::Wildcard)
			.event(flecs::OnSet)
			.each([&world](flecs::iter& it, size_t t) {
			auto event = it.pair(0);
			it.entity(t).children([&world, &event](flecs::entity action) {
				if (action.has<ECS::Action>() && action.has(event))
					action.enable<ECS::Action>(); });
				});

		world.observer<>("TriggerUIInverseAction")
			.with<Event>().second(flecs::Wildcard)
			.event(flecs::OnRemove)
			.each([&world](flecs::iter& it, size_t t) {
			auto event = it.pair(0);
			it.entity(t).children([&world, &event](flecs::entity action) {
				if (action.has<ECS::Invert>() && action.has(event))
					action.enable<ECS::Invert>(); });
				});

		world.observer<>("SetActionParent")
			.with<ECS::Action>()
			.with<ECS::Path>()
			.event(flecs::OnSet)
			.each([](flecs::entity entity) {
			if (entity.has<ECS::Parent>()) {
				FString parentPath = UTF8_TO_TCHAR(entity.parent().path().c_str());
				FString parent = entity.get<ECS::Parent>()->Value;
				if (!parent.IsEmpty())
					parentPath += TEXT(".") + parent;
				using namespace ECS;
				entity.set<Parent>({ NormalizedPath(parentPath) });
			}
				});
	}
}