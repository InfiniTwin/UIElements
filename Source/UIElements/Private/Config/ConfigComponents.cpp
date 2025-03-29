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