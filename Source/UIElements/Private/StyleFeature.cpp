// Fill out your copyright notice in the Description page of Project Settings.

#include "StyleFeature.h"
#include "flecs.h"
#include "ECS.h"
#include "UIFeature.h"
#include "TypographyFeature.h"
#include "ButtonFeature.h"
#include "Styling/SlateStyleRegistry.h"
#include "Engine/UserInterfaceSettings.h"

namespace UI {
	void StyleFeature::RegisterComponents(flecs::world& world) {
		using namespace ECS;
		world.component<ButtonStyle>().add(flecs::OnInstantiate, flecs::Inherit);

		world.component<Brush>().add(flecs::OnInstantiate, flecs::Inherit);
		world.component<BrushType>().member<int>(VALUE).add(flecs::OnInstantiate, flecs::Inherit);

		world.component<Width>().member<float>(VALUE).add(flecs::OnInstantiate, flecs::Inherit);
		world.component<FixedRadius>().add(flecs::OnInstantiate, flecs::Inherit);
		world.component<Radii>()
			.member<float>(MEMBER(Radii::TopLeft))
			.member<float>(MEMBER(Radii::TopRight))
			.member<float>(MEMBER(Radii::BottomRight))
			.member<float>(MEMBER(Radii::BottomLeft))
			.add(flecs::OnInstantiate, flecs::Inherit);
	}

	void StyleFeature::CreateQueries(flecs::world& world) {
		world.component<QueryButtonStylePrefab>();
		world.set(QueryButtonStylePrefab{
			world.query_builder<ButtonStyle>(COMPONENT(QueryButtonStylePrefab))
			.with<Button>()
			.without<Widget>()
			.with(flecs::Prefab)
			.cached().build() });
	};

	void StyleFeature::CreateObservers(flecs::world& world) {
		world.observer<const UIScheme>("UpdateButtonStyles")
			.term_at(0).singleton()
			.event(flecs::OnSet)
			.each([&world](const UIScheme& scheme) {
			world.get<QueryButtonStylePrefab>()->Value
				.each([&world](flecs::entity prefab, ButtonStyle& style) {
				style.Value = AddButtonStyle(prefab);
					});
				});
	}
}