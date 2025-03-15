// Fill out your copyright notice in the Description page of Project Settings.


#include "UI.h"
#include "Theme/ThemeComponents.h"

namespace UI {
	Module::Module(flecs::world& world) {
		world.module<Module>();

		ThemeComponents::Register(world);
	}
}