// Fill out your copyright notice in the Description page of Project Settings.

#include "Config/ConfigSystems.h"
#include "Config/ConfigComponents.h"
#include "Component.h"
#include "dynamiccolor/dynamic_scheme.h"
#include "dynamiccolor/material_dynamic_colors.h"
#include "palettes/tones.h"
#include "UIUtils.h"

namespace UIElements {
	void ConfigSystems::Register(flecs::world& world) {
		Initialize(world);
	}

	void ConfigSystems::Initialize(flecs::world& world) {
		Component::DeserializeSingletons(world, "UIConfig");

		using namespace material_color_utilities;
		auto uiScheme = world.get<UIScheme>();
		auto dynamicScheme = DynamicScheme(
			Hct(uiScheme->Primary),
			uiScheme->Variant,
			uiScheme->Contrast,
			uiScheme->DarkMode,
			TonalPalette(uiScheme->Primary),
			TonalPalette(uiScheme->Secondary),
			TonalPalette(uiScheme->Tertiary),
			TonalPalette(uiScheme->Neutral),
			TonalPalette(uiScheme->NeutralVariant),
			TonalPalette(uiScheme->Error));

		world.set<UIColors>({
			MaterialDynamicColors::Background().GetLinear(dynamicScheme),
			MaterialDynamicColors::OnBackground().GetLinear(dynamicScheme),
			MaterialDynamicColors::Surface().GetLinear(dynamicScheme),
			MaterialDynamicColors::SurfaceDim().GetLinear(dynamicScheme),
			MaterialDynamicColors::SurfaceBright().GetLinear(dynamicScheme),
			MaterialDynamicColors::SurfaceContainerLowest().GetLinear(dynamicScheme),
			MaterialDynamicColors::SurfaceContainerLow().GetLinear(dynamicScheme),
			MaterialDynamicColors::SurfaceContainer().GetLinear(dynamicScheme),
			MaterialDynamicColors::SurfaceContainerHigh().GetLinear(dynamicScheme),
			MaterialDynamicColors::SurfaceContainerHighest().GetLinear(dynamicScheme),
			MaterialDynamicColors::OnSurface().GetLinear(dynamicScheme),
			MaterialDynamicColors::SurfaceVariant().GetLinear(dynamicScheme),
			MaterialDynamicColors::OnSurfaceVariant().GetLinear(dynamicScheme),
			MaterialDynamicColors::InverseSurface().GetLinear(dynamicScheme),
			MaterialDynamicColors::InverseOnSurface().GetLinear(dynamicScheme),
			MaterialDynamicColors::Outline().GetLinear(dynamicScheme),
			MaterialDynamicColors::OutlineVariant().GetLinear(dynamicScheme),
			MaterialDynamicColors::Shadow().GetLinear(dynamicScheme),
			MaterialDynamicColors::Scrim().GetLinear(dynamicScheme),
			MaterialDynamicColors::SurfaceTint().GetLinear(dynamicScheme),
			MaterialDynamicColors::Primary().GetLinear(dynamicScheme),
			MaterialDynamicColors::OnPrimary().GetLinear(dynamicScheme),
			MaterialDynamicColors::PrimaryContainer().GetLinear(dynamicScheme),
			MaterialDynamicColors::OnPrimaryContainer().GetLinear(dynamicScheme),
			MaterialDynamicColors::InversePrimary().GetLinear(dynamicScheme),
			MaterialDynamicColors::Secondary().GetLinear(dynamicScheme),
			MaterialDynamicColors::OnSecondary().GetLinear(dynamicScheme),
			MaterialDynamicColors::SecondaryContainer().GetLinear(dynamicScheme),
			MaterialDynamicColors::OnSecondaryContainer().GetLinear(dynamicScheme),
			MaterialDynamicColors::Tertiary().GetLinear(dynamicScheme),
			MaterialDynamicColors::OnTertiary().GetLinear(dynamicScheme),
			MaterialDynamicColors::TertiaryContainer().GetLinear(dynamicScheme),
			MaterialDynamicColors::OnTertiaryContainer().GetLinear(dynamicScheme),
			MaterialDynamicColors::Error().GetLinear(dynamicScheme),
			MaterialDynamicColors::OnError().GetLinear(dynamicScheme),
			MaterialDynamicColors::ErrorContainer().GetLinear(dynamicScheme),
			MaterialDynamicColors::OnErrorContainer().GetLinear(dynamicScheme),
			MaterialDynamicColors::PrimaryFixed().GetLinear(dynamicScheme),
			MaterialDynamicColors::PrimaryFixedDim().GetLinear(dynamicScheme),
			MaterialDynamicColors::OnPrimaryFixed().GetLinear(dynamicScheme),
			MaterialDynamicColors::OnPrimaryFixedVariant().GetLinear(dynamicScheme),
			MaterialDynamicColors::SecondaryFixed().GetLinear(dynamicScheme),
			MaterialDynamicColors::SecondaryFixedDim().GetLinear(dynamicScheme),
			MaterialDynamicColors::OnSecondaryFixed().GetLinear(dynamicScheme),
			MaterialDynamicColors::OnSecondaryFixedVariant().GetLinear(dynamicScheme),
			MaterialDynamicColors::TertiaryFixed().GetLinear(dynamicScheme),
			MaterialDynamicColors::TertiaryFixedDim().GetLinear(dynamicScheme),
			MaterialDynamicColors::OnTertiaryFixed().GetLinear(dynamicScheme),
			MaterialDynamicColors::OnTertiaryFixedVariant().GetLinear(dynamicScheme) });

		// Read from config 
		//FString FontPath = TEXT("Fonts"); // get from config
		//auto font400 = TEXT("Font400");
		//auto font500 = TEXT("Font500");

		//auto uiScale = world.get<UIScale>()->Value;
		//world.set <TextTypes>({
		//	FSlateFontInfo(font400, 57 * uiScale), // DisplayLarge
		//	FSlateFontInfo(font400, 45 * uiScale), // DisplayMedium
		//	FSlateFontInfo(font400, 36 * uiScale), // DisplaySmall
		//	FSlateFontInfo(font400, 32 * uiScale), // HeadlineLarge
		//	FSlateFontInfo(font400, 28 * uiScale), // HeadlineMedium
		//	FSlateFontInfo(font400, 24 * uiScale), // HeadlineSmall
		//	FSlateFontInfo(font400, 22 * uiScale), // TitleLarge
		//	FSlateFontInfo(font500, 16 * uiScale), // TitleMedium
		//	FSlateFontInfo(font500, 14 * uiScale), // TitleSmall
		//	FSlateFontInfo(font400, 14 * uiScale), // BodyLarge
		//	FSlateFontInfo(font400, 12 * uiScale), // BodyMedium
		//	FSlateFontInfo(font400, 11 * uiScale), // BodySmall
		//	FSlateFontInfo(font500, 16 * uiScale), // LabelLarge
		//	FSlateFontInfo(font500, 14 * uiScale), // LabelMedium
		//	FSlateFontInfo(font500, 12 * uiScale)	// LabelSmall
		//	});

		//world.set<IconFont>({ IconFont() });
		//world.set<IconFontFill>({ IconFontFill() });
	}
}
