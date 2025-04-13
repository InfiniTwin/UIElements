// Fill out your copyright notice in the Description page of Project Settings.

#include "ColorFeature.h"
#include "UIUtils.h"
#include "ECS.h"
#include "dynamiccolor/dynamic_scheme.h"
#include "dynamiccolor/material_dynamic_colors.h"
#include "palettes/tones.h"

namespace UIElements {
	void ColorFeature::RegisterOpaqueTypes(flecs::world& world) {
		// MCU Variant <=> int
		using namespace material_color_utilities;
		world.component<Variant>()
			.opaque(flecs::I32)
			.serialize([](const flecs::serializer* s, const Variant* data)
				{
					int value = static_cast<int>(*data);
					return s->value(flecs::I32, &value);
				})
			.assign_int([](Variant* data, int64_t value) {
			if (value >= static_cast<int64_t>(Variant::kMonochrome) &&
				value <= static_cast<int64_t>(Variant::kFruitSalad)) {
				*data = static_cast<Variant>(value);
			}
			else {
				*data = Variant::kMonochrome;
			}
				});

		// Argb <=> Hex
		world.component<Argb>()
			.opaque(flecs::String)
			.serialize([](const flecs::serializer* s, const Argb* data)
				{
					std::string hex = UIUtils::HexFromArgb(*data);
					const char* str = hex.c_str();
					return s->value(flecs::String, &str);
				})
			.assign_string([](Argb* data, const char* value)
				{
					*data = UIUtils::ArgbFromHex(value);
				});
	}

	void ColorFeature::RegisterComponents(flecs::world& world) {
		world.component<UIScheme>()
			.member<bool>(MEMBER(UIScheme::DarkMode))
			.member<double>(MEMBER(UIScheme::Contrast))
			.member<Variant>(MEMBER(UIScheme::Variant))
			.member<Argb>(MEMBER(UIScheme::Primary))
			.member<Argb>(MEMBER(UIScheme::Secondary))
			.member<Argb>(MEMBER(UIScheme::Tertiary))
			.member<Argb>(MEMBER(UIScheme::Neutral))
			.member<Argb>(MEMBER(UIScheme::NeutralVariant))
			.member<Argb>(MEMBER(UIScheme::Error));

		world.component<UIColors>()
			.member<FLinearColor>(MEMBER(UIColors::Background))
			.member<FLinearColor>(MEMBER(UIColors::OnBackground))
			.member<FLinearColor>(MEMBER(UIColors::Surface))
			.member<FLinearColor>(MEMBER(UIColors::SurfaceDim))
			.member<FLinearColor>(MEMBER(UIColors::SurfaceBright))
			.member<FLinearColor>(MEMBER(UIColors::SurfaceContainerLowest))
			.member<FLinearColor>(MEMBER(UIColors::SurfaceContainerLow))
			.member<FLinearColor>(MEMBER(UIColors::SurfaceContainer))
			.member<FLinearColor>(MEMBER(UIColors::SurfaceContainerHigh))
			.member<FLinearColor>(MEMBER(UIColors::SurfaceContainerHighest))
			.member<FLinearColor>(MEMBER(UIColors::OnSurface))
			.member<FLinearColor>(MEMBER(UIColors::SurfaceVariant))
			.member<FLinearColor>(MEMBER(UIColors::OnSurfaceVariant))
			.member<FLinearColor>(MEMBER(UIColors::InverseSurface))
			.member<FLinearColor>(MEMBER(UIColors::InverseOnSurface))
			.member<FLinearColor>(MEMBER(UIColors::Outline))
			.member<FLinearColor>(MEMBER(UIColors::OutlineVariant))
			.member<FLinearColor>(MEMBER(UIColors::Shadow))
			.member<FLinearColor>(MEMBER(UIColors::Scrim))
			.member<FLinearColor>(MEMBER(UIColors::SurfaceTint))
			.member<FLinearColor>(MEMBER(UIColors::Primary))
			.member<FLinearColor>(MEMBER(UIColors::OnPrimary))
			.member<FLinearColor>(MEMBER(UIColors::PrimaryContainer))
			.member<FLinearColor>(MEMBER(UIColors::OnPrimaryContainer))
			.member<FLinearColor>(MEMBER(UIColors::InversePrimary))
			.member<FLinearColor>(MEMBER(UIColors::Secondary))
			.member<FLinearColor>(MEMBER(UIColors::OnSecondary))
			.member<FLinearColor>(MEMBER(UIColors::SecondaryContainer))
			.member<FLinearColor>(MEMBER(UIColors::OnSecondaryContainer))
			.member<FLinearColor>(MEMBER(UIColors::Tertiary))
			.member<FLinearColor>(MEMBER(UIColors::OnTertiary))
			.member<FLinearColor>(MEMBER(UIColors::TertiaryContainer))
			.member<FLinearColor>(MEMBER(UIColors::OnTertiaryContainer))
			.member<FLinearColor>(MEMBER(UIColors::Error))
			.member<FLinearColor>(MEMBER(UIColors::OnError))
			.member<FLinearColor>(MEMBER(UIColors::ErrorContainer))
			.member<FLinearColor>(MEMBER(UIColors::OnErrorContainer))
			.member<FLinearColor>(MEMBER(UIColors::PrimaryFixed))
			.member<FLinearColor>(MEMBER(UIColors::PrimaryFixedDim))
			.member<FLinearColor>(MEMBER(UIColors::OnPrimaryFixed))
			.member<FLinearColor>(MEMBER(UIColors::OnPrimaryFixedVariant))
			.member<FLinearColor>(MEMBER(UIColors::SecondaryFixed))
			.member<FLinearColor>(MEMBER(UIColors::SecondaryFixedDim))
			.member<FLinearColor>(MEMBER(UIColors::OnSecondaryFixed))
			.member<FLinearColor>(MEMBER(UIColors::OnSecondaryFixedVariant))
			.member<FLinearColor>(MEMBER(UIColors::TertiaryFixed))
			.member<FLinearColor>(MEMBER(UIColors::TertiaryFixedDim))
			.member<FLinearColor>(MEMBER(UIColors::OnTertiaryFixed))
			.member<FLinearColor>(MEMBER(UIColors::OnTertiaryFixedVariant));
	}

	void ColorFeature::RegisterSystems(flecs::world& world) {
	}

	void ColorFeature::Initialize(flecs::world& world) {
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
	}
}