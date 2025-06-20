// Fill out your copyright notice in the Description page of Project Settings.

#include "ButtonFeature.h"
#include "WidgetFeature.h"

namespace UI {
	void ButtonFeature::RegisterComponents(flecs::world& world) {
		world.component<Button>().add(flecs::OnInstantiate, flecs::Inherit);
		world.component<Toggle>().add(flecs::OnInstantiate, flecs::Inherit);
		world.component<ButtonStyle>().add(flecs::OnInstantiate, flecs::Inherit);

		world.component<CheckBox>().add(flecs::OnInstantiate, flecs::Inherit);
		world.component<CheckBoxStyle>().add(flecs::OnInstantiate, flecs::Inherit);
	};

	void ButtonFeature::CreateQueries(flecs::world& world) {
		world.component<QueryButtonStylePrefab>();
		world.set(QueryButtonStylePrefab{
			world.query_builder<ButtonStyle>(COMPONENT(QueryButtonStylePrefab))
			.with<Button>()
			.without<Widget>()
			.with(flecs::Prefab)
			.cached().build() });

		world.component<QueryCheckBoxStylePrefab>();
		world.set(QueryCheckBoxStylePrefab{
			world.query_builder<CheckBoxStyle>(COMPONENT(QueryCheckBoxStylePrefab))
			.with<CheckBox>()
			.without<Widget>()
			.with(flecs::Prefab)
			.cached().build() });
	};

	void ButtonFeature::CreateObservers(flecs::world& world) {
		//world.observer<const UIScheme>("UpdateButtonStyles")
		//	.term_at(0).singleton()
		//	.event(flecs::OnSet)
		//	.each([&world](const UIScheme& scheme) {
		//	world.get<QueryButtonStylePrefab>()->Value
		//		.each([](flecs::entity prefab, ButtonStyle& style) {
		//		SetButtonStyle(prefab);
		//			});

		//	world.get<QueryCheckBoxStylePrefab>()->Value
		//		.each([](flecs::entity prefab, CheckBoxStyle& style) {
		//		SetCheckBoxStyle(prefab);
		//			});
		//		});
	}
}