// Fill out your copyright notice in the Description page of Project Settings.


#include "ButtonFeature.h"

namespace UIElements {
	void ButtonFeature::RegisterComponents(flecs::world& world) {
		world.component<Button>();
	};
}