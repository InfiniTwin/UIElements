// Fill out your copyright notice in the Description page of Project Settings.

#include "Config/ConfigComponents.h"
#include "UIUtils.h"
#include "Component.h"

namespace UIElements {
	void ConfigComponents::Register(flecs::world& world) {
		RegisterOpaqueTypes(world);

		world.component<UIScale>().member<double>(NAMEOF_MEMBER(UIScale::Value));

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

		world.component<TextFont>().member<FString>(NAMEOF_MEMBER(TextFont::Value));
	}

	void ConfigComponents::RegisterOpaqueTypes(flecs::world& world) {
		// FString <=> std::string
		world.component<FString>()
			.opaque(flecs::String)
			.serialize([](const flecs::serializer* s, const FString* data)
				{
					const char* str = TCHAR_TO_UTF8(**data);
					return s->value(flecs::String, &str);
				})
			.assign_string([](FString* data, const char* value)
				{
					*data = UTF8_TO_TCHAR(value);
				});

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
}