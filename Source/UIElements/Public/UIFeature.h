// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "flecs.h"

namespace UIElements {
	struct UIScale { double Value; };

	struct UIFeature {
		static void RegisterComponents(flecs::world& world);
		static void CreateObservers(flecs::world& world);
	};
}