// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "flecs.h"
#include "dynamiccolor/variant.h"
#include "utils/utils.h"

namespace UIElements {
	struct Color { FLinearColor Value; };
	struct SyncedColor {};

	using namespace material_color_utilities;
	struct UIScheme {
		bool DarkMode;
		double Contrast;
		Variant Variant;
		Argb Primary;
		Argb Secondary;
		Argb Tertiary;
		Argb Neutral;
		Argb NeutralVariant;
		Argb Error;
	};

	struct ColorPrefabQuery { flecs::query<Color> Value; };

	struct ColorFeature {
		static void RegisterOpaqueTypes(flecs::world& world);
		static void RegisterComponents(flecs::world& world);
		static void CreateQueries(flecs::world& world);
		static void CreateObservers(flecs::world& world);
		static void CreateSystems(flecs::world& world);

	};

	void SetPrefabColor(flecs::world& world, const FString name, const FLinearColor color);
}