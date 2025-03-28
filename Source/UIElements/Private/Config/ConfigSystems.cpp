// Fill out your copyright notice in the Description page of Project Settings.

#include "Config/ConfigSystems.h"
#include "Config/ConfigComponents.h"
#include "dynamiccolor/dynamic_scheme.h"
#include "dynamiccolor/material_dynamic_colors.h"
#include "palettes/tones.h"
#include "UIUtils.h"

namespace UIElements {
	void ConfigSystems::Register(flecs::world& world) {
		Initialize(world);
	}

	void ConfigSystems::Initialize(flecs::world& world) {
		using namespace material_color_utilities;
		auto scheme = DynamicScheme(
			/*source_color_argb=*/ Hct(0xFF1B6EF3),
			/*variant=*/ Variant::kFidelity,
			/*contrast_level=*/ 0.5,
			/*is_dark=*/ false,
			/*primary_palette=*/ TonalPalette(0xFF1B6EF3),
			/*secondary_palette=*/ TonalPalette(0xFF8E909B),
			/*tertiary_palette=*/ TonalPalette(0xFF4A90E2),
			/*neutral_palette=*/ TonalPalette(0xFF919094),
			/*neutral_variant_palette=*/ TonalPalette(0xFF8F9099),
			/*error_palette=*/ TonalPalette(0xFFAB342C));

		Argb argb = MaterialDynamicColors::Surface().GetArgb(scheme);
		//UE_LOG(LogTemp, Log, TEXT(">>> %u"), argb);
		//std::stringstream ss;
		//ss << "0x" << std::setw(8) << std::setfill('0') << std::hex << argb;
		//UE_LOG(LogTemp, Log, TEXT(">>> %s"), *FString(ss.str().c_str()));
		UE_LOG(LogTemp, Log, TEXT(">>> %hs"), UIUtils::HexFromArgb(argb).c_str());
		

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
