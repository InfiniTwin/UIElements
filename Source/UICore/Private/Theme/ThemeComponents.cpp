// Fill out your copyright notice in the Description page of Project Settings.


#include "Theme/ThemeComponents.h"
#include "flecs.h"

namespace UI {

	void ThemeComponents::Register(flecs::world& world) {
		world.component<UIScale>().member<float>("Value");		
	}
}