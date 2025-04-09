// Fill out your copyright notice in the Description page of Project Settings.

#include "ColorFeature.h"
#include "UIUtils.h"
#include "Component.h"
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
			.member<bool>(NAMEOF_MEMBER(UIScheme::DarkMode))
			.member<double>(NAMEOF_MEMBER(UIScheme::Contrast))
			.member<Variant>(NAMEOF_MEMBER(UIScheme::Variant))
			.member<Argb>(NAMEOF_MEMBER(UIScheme::Primary))
			.member<Argb>(NAMEOF_MEMBER(UIScheme::Secondary))
			.member<Argb>(NAMEOF_MEMBER(UIScheme::Tertiary))
			.member<Argb>(NAMEOF_MEMBER(UIScheme::Neutral))
			.member<Argb>(NAMEOF_MEMBER(UIScheme::NeutralVariant))
			.member<Argb>(NAMEOF_MEMBER(UIScheme::Error));

		world.component<UIColors>()
			.member<FLinearColor>(NAMEOF_MEMBER(UIColors::Background))
			.member<FLinearColor>(NAMEOF_MEMBER(UIColors::OnBackground))
			.member<FLinearColor>(NAMEOF_MEMBER(UIColors::Surface))
			.member<FLinearColor>(NAMEOF_MEMBER(UIColors::SurfaceDim))
			.member<FLinearColor>(NAMEOF_MEMBER(UIColors::SurfaceBright))
			.member<FLinearColor>(NAMEOF_MEMBER(UIColors::SurfaceContainerLowest))
			.member<FLinearColor>(NAMEOF_MEMBER(UIColors::SurfaceContainerLow))
			.member<FLinearColor>(NAMEOF_MEMBER(UIColors::SurfaceContainer))
			.member<FLinearColor>(NAMEOF_MEMBER(UIColors::SurfaceContainerHigh))
			.member<FLinearColor>(NAMEOF_MEMBER(UIColors::SurfaceContainerHighest))
			.member<FLinearColor>(NAMEOF_MEMBER(UIColors::OnSurface))
			.member<FLinearColor>(NAMEOF_MEMBER(UIColors::SurfaceVariant))
			.member<FLinearColor>(NAMEOF_MEMBER(UIColors::OnSurfaceVariant))
			.member<FLinearColor>(NAMEOF_MEMBER(UIColors::InverseSurface))
			.member<FLinearColor>(NAMEOF_MEMBER(UIColors::InverseOnSurface))
			.member<FLinearColor>(NAMEOF_MEMBER(UIColors::Outline))
			.member<FLinearColor>(NAMEOF_MEMBER(UIColors::OutlineVariant))
			.member<FLinearColor>(NAMEOF_MEMBER(UIColors::Shadow))
			.member<FLinearColor>(NAMEOF_MEMBER(UIColors::Scrim))
			.member<FLinearColor>(NAMEOF_MEMBER(UIColors::SurfaceTint))
			.member<FLinearColor>(NAMEOF_MEMBER(UIColors::Primary))
			.member<FLinearColor>(NAMEOF_MEMBER(UIColors::OnPrimary))
			.member<FLinearColor>(NAMEOF_MEMBER(UIColors::PrimaryContainer))
			.member<FLinearColor>(NAMEOF_MEMBER(UIColors::OnPrimaryContainer))
			.member<FLinearColor>(NAMEOF_MEMBER(UIColors::InversePrimary))
			.member<FLinearColor>(NAMEOF_MEMBER(UIColors::Secondary))
			.member<FLinearColor>(NAMEOF_MEMBER(UIColors::OnSecondary))
			.member<FLinearColor>(NAMEOF_MEMBER(UIColors::SecondaryContainer))
			.member<FLinearColor>(NAMEOF_MEMBER(UIColors::OnSecondaryContainer))
			.member<FLinearColor>(NAMEOF_MEMBER(UIColors::Tertiary))
			.member<FLinearColor>(NAMEOF_MEMBER(UIColors::OnTertiary))
			.member<FLinearColor>(NAMEOF_MEMBER(UIColors::TertiaryContainer))
			.member<FLinearColor>(NAMEOF_MEMBER(UIColors::OnTertiaryContainer))
			.member<FLinearColor>(NAMEOF_MEMBER(UIColors::Error))
			.member<FLinearColor>(NAMEOF_MEMBER(UIColors::OnError))
			.member<FLinearColor>(NAMEOF_MEMBER(UIColors::ErrorContainer))
			.member<FLinearColor>(NAMEOF_MEMBER(UIColors::OnErrorContainer))
			.member<FLinearColor>(NAMEOF_MEMBER(UIColors::PrimaryFixed))
			.member<FLinearColor>(NAMEOF_MEMBER(UIColors::PrimaryFixedDim))
			.member<FLinearColor>(NAMEOF_MEMBER(UIColors::OnPrimaryFixed))
			.member<FLinearColor>(NAMEOF_MEMBER(UIColors::OnPrimaryFixedVariant))
			.member<FLinearColor>(NAMEOF_MEMBER(UIColors::SecondaryFixed))
			.member<FLinearColor>(NAMEOF_MEMBER(UIColors::SecondaryFixedDim))
			.member<FLinearColor>(NAMEOF_MEMBER(UIColors::OnSecondaryFixed))
			.member<FLinearColor>(NAMEOF_MEMBER(UIColors::OnSecondaryFixedVariant))
			.member<FLinearColor>(NAMEOF_MEMBER(UIColors::TertiaryFixed))
			.member<FLinearColor>(NAMEOF_MEMBER(UIColors::TertiaryFixedDim))
			.member<FLinearColor>(NAMEOF_MEMBER(UIColors::OnTertiaryFixed))
			.member<FLinearColor>(NAMEOF_MEMBER(UIColors::OnTertiaryFixedVariant));
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