// Fill out your copyright notice in the Description page of Project Settings.

#include "UIFeature.h"
#include <ECS.h>
#include <ActionFeature.h>
#include <Engine/UserInterfaceSettings.h>

namespace UI {
	void UIFeature::RegisterComponents(flecs::world& world) {
		using namespace ECS;
		world.component<UIScale>().member<float>(VALUE);

		world.component<WidgetState>().add(flecs::Exclusive);
		world.component<ButtonState>().add(flecs::Exclusive);
		world.component<CheckBoxState>().add(flecs::Exclusive);
	}

	void UIFeature::CreateObservers(flecs::world& world) {
		world.observer<const UIScale>("UpdateApplicationUIScale")
			.term_at(0).singleton()
			.event(flecs::OnSet)
			.each([&world](const UIScale& scale) {
			GetMutableDefault<UUserInterfaceSettings>(UUserInterfaceSettings::StaticClass())
				->ApplicationScale = scale.Value; });

		world.observer<>("TriggerWidgetAction")
			.with<WidgetState>().second(flecs::Wildcard)
			.or_()
			.with<ButtonState>().second(flecs::Wildcard)
			.or_()
			.with<CheckBoxState>().second(flecs::Wildcard)
			.event(flecs::OnSet)
			.each([&world](flecs::iter& it, size_t t) {
			auto event = it.pair(0);
			it.entity(t).children([&world, &event](flecs::entity action) {
				if (action.has<ECS::Action>() && action.has(event))
					action.enable<ECS::Action>(); });
				});

		world.observer<>("TriggerWidgetInverseAction")
			.with<WidgetState>().second(flecs::Wildcard)
			.or_()
			.with<ButtonState>().second(flecs::Wildcard)
			.or_()
			.with<CheckBoxState>().second(flecs::Wildcard)
			.event(flecs::OnRemove)
			.each([&world](flecs::iter& it, size_t t) {
			auto event = it.pair(0);
			it.entity(t).children([&world, &event](flecs::entity action) {
				if (action.has<ECS::Invert>() && action.has(event))
					action.enable<ECS::Invert>(); });
				});
	}
}