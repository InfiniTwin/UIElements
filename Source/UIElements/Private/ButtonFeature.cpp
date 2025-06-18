// Fill out your copyright notice in the Description page of Project Settings.

#include "ButtonFeature.h"
#include "WidgetFeature.h"

namespace UI {
	void ButtonFeature::RegisterComponents(flecs::world& world) {
		world.component<Border>().add(flecs::OnInstantiate, flecs::Inherit);
		world.component<Button>().add(flecs::OnInstantiate, flecs::Inherit);
		world.component<Toggle>();

		world.component<ButtonStyle>().add(flecs::OnInstantiate, flecs::Inherit);
	};

	void ButtonFeature::CreateQueries(flecs::world& world) {
		world.component<QueryButtonStylePrefab>();
		world.set(QueryButtonStylePrefab{
			world.query_builder<ButtonStyle>(COMPONENT(QueryButtonStylePrefab))
			.with<Button>()
			.without<Widget>()
			.with(flecs::Prefab)
			.cached().build() });
	};

	void ButtonFeature::CreateObservers(flecs::world& world) {
		world.observer<const UIScheme>("UpdateButtonStyle")
			.term_at(0).singleton()
			.event(flecs::OnSet)
			.each([&world](const UIScheme& scheme) {
			world.get<QueryButtonStylePrefab>()->Value
				.each([&world](flecs::entity prefab, ButtonStyle& style) {
				style.Value = GetButtonStyle(prefab);
					});
				});
	}
}