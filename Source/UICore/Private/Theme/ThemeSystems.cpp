// Fill out your copyright notice in the Description page of Project Settings.


#include "Theme/ThemeSystems.h"
#include "Theme/ThemeComponents.h"
#include "flecs.h"

namespace UI {

	void ThemeSystems::Register(flecs::world& world) {
		Initialize(world);
	}

	void ThemeSystems::Initialize(flecs::world& world) {
		world.set<UIScale>({2.2f});
	}
}