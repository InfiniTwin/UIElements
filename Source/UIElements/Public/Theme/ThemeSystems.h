// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "flecs.h"
#include "Theme/ThemeComponents.h"

namespace UIElements {

	struct ThemeSystems
	{
	public:
		static void Register(flecs::world& world);

	private:
		static void Initialize(flecs::world& world);
	};
}