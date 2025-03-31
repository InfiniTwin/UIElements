// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "flecs.h"

namespace UIElements {
	struct UIFeature {
		static void RegisterOpaqueTypes(flecs::world& world);
		static void RegisterComponents(flecs::world& world);
	};

	struct UIScale { double Value; };

	struct Delay {
		float RemainingTime;
		std::function<void()> Callback;
	};
}