// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "flecs.h"
#include "dynamiccolor/variant.h"
#include "utils/utils.h"

namespace UIElements {
	struct ConfigComponents {
		static void Register(flecs::world& world);
	};

#pragma region Singletons

	struct UIScale { double Value = 1; };

	using namespace material_color_utilities;
	struct UIScheme {
		bool DarkMode;
		double Contrast;
		Variant Variant;
		Argb PrimaryColor;
		Argb SecondaryColor;
		Argb TertiaryColor;
		Argb NuetralColor;
		Argb NeutralVariantColor;
		Argb ErrorColor;
	};

	struct Colors {
		// Primary Colors
		FString Primary;
		FString OnPrimary;
		FString PrimaryContainer;
		FString OnPrimaryContainer;

		// Secondary Colors
		FString Secondary;
		FString OnSecondary;
		FString SecondaryContainer;
		FString OnSecondaryContainer;

		// Tertiary Colors
		FString Tertiary;
		FString OnTertiary;
		FString TertiaryContainer;
		FString OnTertiaryContainer;

		// Error Colors
		FString Error;
		FString OnError;
		FString ErrorContainer;
		FString OnErrorContainer;

		// Fixed Colors
		FString PrimaryFixed;
		FString PrimaryFixedDim;
		FString OnPrimaryFixed;
		FString OnPrimaryFixedVariant;

		FString SecondaryFixed;
		FString SecondaryFixedDim;
		FString OnSecondaryFixed;
		FString OnSecondaryFixedVariant;

		FString TertiaryFixed;
		FString TertiaryFixedDim;
		FString OnTertiaryFixed;
		FString OnTertiaryFixedVariant;

		// Surface Colors
		FString SurfaceDim;
		FString Surface;
		FString SurfaceBright;

		FString SurfaceContainerLowest;
		FString SurfaceContainerLow;
		FString SurfaceContainer;
		FString SurfaceContainerHigh;
		FString SurfaceContainerHighest;

		// Other Colors
		FString OnSurface;
		FString OnSurfaceVariant;
		FString Outline;
		FString OutlineVariant;

		FString InverseSurface;
		FString InverseOnSurface;
		FString InversePrimary;

		FString Scrim;
		FString Shadow;
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