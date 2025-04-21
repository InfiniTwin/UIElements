// Fill out your copyright notice in the Description page of Project Settings.


#include "ToggleFeature.h"

namespace UIElements {
	void ToggleFeature::RegisterComponents(flecs::world& world) {
		world.component<Toggle>();
	};
}