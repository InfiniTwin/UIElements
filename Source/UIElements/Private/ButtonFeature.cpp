// Fill out your copyright notice in the Description page of Project Settings.

#include "ButtonFeature.h"
#include "WidgetFeature.h"

namespace UI {
	void ButtonFeature::RegisterComponents(flecs::world& world) {
		world.component<Border>().add(flecs::OnInstantiate, flecs::Inherit);
		world.component<Button>().add(flecs::OnInstantiate, flecs::Inherit);
	};

	void ButtonFeature::CreateSystems(flecs::world& world) {
	}
}