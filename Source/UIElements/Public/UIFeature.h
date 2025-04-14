// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "flecs.h"

inline constexpr TCHAR DefaultUIStyle[] = TEXT("UIStyle");

namespace UIElements {
	struct UIFeature {
		static void RegisterComponents(flecs::world& world);
		static void Initialize(flecs::world& world);
	};

	struct UIScale { double Value; };

	struct Delay {
		float RemainingTime;
		std::function<void()> Callback;
	};
}