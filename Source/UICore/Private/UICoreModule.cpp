// Fill out your copyright notice in the Description page of Project Settings.


#include "UICoreModule.h"
#include "Theme/ThemeComponents.h"
#include "Theme/ThemeSystems.h"

namespace UI {
	UICoreModule::UICoreModule(flecs::world& world) {
		world.module<UICoreModule>();

		ThemeComponents::Register(world);
		ThemeSystems::Register(world);
	}
}