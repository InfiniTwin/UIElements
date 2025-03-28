// Fill out your copyright notice in the Description page of Project Settings.

#include "Config/ConfigComponents.h"
#include "UIUtils.h"
#include "World.h"

namespace UIElements {
	void ConfigComponents::Register(flecs::world& world) {
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

		world.component<UIScale>().member<double>("Value");
		world.component<UIScheme>()
			.member<bool>("DarkMode")
			.member<double>("Contrast")
			.member<Variant>("Variant")
			.member<Argb>("PrimaryColor")
			.member<Argb>("SecondaryColor")
			.member<Argb>("TertiaryColor")
			.member<Argb>("NuetralColor")
			.member<Argb>("NeutralVariantColor")
			.member<Argb>("ErrorColor");

		world.component<TextFont>().member<FString>("Value");

		World::DeserializeSingletons(world, "UIConfig");
	}
}