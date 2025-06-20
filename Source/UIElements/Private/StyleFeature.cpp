// Fill out your copyright notice in the Description page of Project Settings.

#include "StyleFeature.h"
#include "flecs.h"

namespace UI {
	void StyleFeature::RegisterComponents(flecs::world& world) {
		using namespace ECS;
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
}