// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "flecs.h"
#include "dynamiccolor/variant.h"
#include "utils/utils.h"

namespace UIElements {
	struct ConfigComponents {
		static void RegisterOpaqueTypes(flecs::world& world);
		static void Register(flecs::world& world);
	};

#pragma region Singletons

	struct UIScale { double Value = 1; };

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

	struct UIColors {
		FLinearColor Background;
		FLinearColor OnBackground;
		FLinearColor Surface;
		FLinearColor SurfaceDim;
		FLinearColor SurfaceBright;
		FLinearColor SurfaceContainerLowest;
		FLinearColor SurfaceContainerLow;
		FLinearColor SurfaceContainer;
		FLinearColor SurfaceContainerHigh;
		FLinearColor SurfaceContainerHighest;
		FLinearColor OnSurface;
		FLinearColor SurfaceVariant;
		FLinearColor OnSurfaceVariant;
		FLinearColor InverseSurface;
		FLinearColor InverseOnSurface;
		FLinearColor Outline;
		FLinearColor OutlineVariant;
		FLinearColor Shadow;
		FLinearColor Scrim;
		FLinearColor SurfaceTint;
		FLinearColor Primary;
		FLinearColor OnPrimary;
		FLinearColor PrimaryContainer;
		FLinearColor OnPrimaryContainer;
		FLinearColor InversePrimary;
		FLinearColor Secondary;
		FLinearColor OnSecondary;
		FLinearColor SecondaryContainer;
		FLinearColor OnSecondaryContainer;
		FLinearColor Tertiary;
		FLinearColor OnTertiary;
		FLinearColor TertiaryContainer;
		FLinearColor OnTertiaryContainer;
		FLinearColor Error;
		FLinearColor OnError;
		FLinearColor ErrorContainer;
		FLinearColor OnErrorContainer;
		FLinearColor PrimaryFixed;
		FLinearColor PrimaryFixedDim;
		FLinearColor OnPrimaryFixed;
		FLinearColor OnPrimaryFixedVariant;
		FLinearColor SecondaryFixed;
		FLinearColor SecondaryFixedDim;
		FLinearColor OnSecondaryFixed;
		FLinearColor OnSecondaryFixedVariant;
		FLinearColor TertiaryFixed;
		FLinearColor TertiaryFixedDim;
		FLinearColor OnTertiaryFixed;
		FLinearColor OnTertiaryFixedVariant;
	};

	// https://m3.material.io/styles/typography/type-scale-tokens
	struct TextFont { FString Value; };
	struct TextTypes {
		// Display
		FSlateFontInfo DisplayLarge;
		FSlateFontInfo DisplayMedium;
		FSlateFontInfo DisplaySmall;

		// Headline
		FSlateFontInfo HeadlineLarge;
		FSlateFontInfo HeadlineMedium;
		FSlateFontInfo HeadlineSmall;

		// Title
		FSlateFontInfo TitleLarge;
		FSlateFontInfo TitleMedium;
		FSlateFontInfo TitleSmall;

		// Body
		FSlateFontInfo BodyLarge;
		FSlateFontInfo BodyMedium;
		FSlateFontInfo BodySmall;

		// Label
		FSlateFontInfo LabelLarge;
		FSlateFontInfo LabelMedium;
		FSlateFontInfo LabelSmall;
	};

	struct IconFont { FString Value; };
	struct IconFontFill { FString Value; };

#pragma endregion
}