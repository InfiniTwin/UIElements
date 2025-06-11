// Fill out your copyright notice in the Description page of Project Settings.


#include "ToggleFeature.h"

namespace UI {
	void ToggleFeature::RegisterComponents(flecs::world& world) {
		world.component<Toggle>();
	};
}