// Fill out your copyright notice in the Description page of Project Settings.


#include "Module.h"
#include "Theme/ThemeComponents.h"
#include "Theme/ThemeSystems.h"

namespace UIElements {
	Module::Module(flecs::world& world) {
		world.module<Module>();

		ThemeComponents::Register(world);
		ThemeSystems::Register(world);
	}
}