// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "flecs.h"
#include "dynamiccolor/variant.h"
#include "utils/utils.h"

namespace UIElements {
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

	struct ColorPrefabQuery { flecs::query<Color> Value; };

	struct ColorFeature {
		static void RegisterOpaqueTypes(flecs::world& world);
		static void RegisterComponents(flecs::world& world);
		static void CreateQueries(flecs::world& world);
		static void CreateObservers(flecs::world& world);

		static inline void SetColor(flecs::world& world, const FString name, const FLinearColor color) {
			auto cn = TEXT("Color") + name;
			FTCHARToUTF8 colorName(*cn);
			world.get<ColorPrefabQuery>()->Value.run([&colorName, &color](flecs::iter& it) {
				while (it.next())
					for (auto i : it)
						if (std::strcmp(it.entity(i).name(), colorName.Get()) == 0)
						{
							it.entity(i).set<Color>({ color });
							return;
						}
				});
		}
	};
}