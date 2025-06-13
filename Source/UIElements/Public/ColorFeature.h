// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "flecs.h"
#include "dynamiccolor/variant.h"
#include "utils/utils.h"

namespace UI {
	struct ColorFeature {
		static void RegisterOpaqueTypes(flecs::world& world);
		static void RegisterComponents(flecs::world& world);
		static void CreateQueries(flecs::world& world);
		static void CreateObservers(flecs::world& world);
	};

	struct Color { FLinearColor Value; };

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

	struct QueryColorPrefab { flecs::query<Color> Value; };

	void SetPrefabColor(flecs::world& world, const FString name, const FLinearColor color);
	void SetWidgetColor(flecs::world& world, flecs::entity entity, const FLinearColor color);
}