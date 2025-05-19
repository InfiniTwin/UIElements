// Fill out your copyright notice in the Description page of Project Settings.

#include "ColorFeature.h"
#include "WidgetFeature.h"
#include "TypographyFeature.h"
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

		// FLinearColor <=> Hex
		world.component<FLinearColor>()
			.opaque(flecs::String)
			.serialize([](const flecs::serializer* s, const FLinearColor* data)
				{
					std::string hex = UIUtils::HexFromLinearColor(*data);
					const char* str = hex.c_str();
					return s->value(flecs::String, &str);
				})
			.assign_string([](FLinearColor* data, const char* value)
				{
					*data = UIUtils::LinearColorFromHex(value);
				});
	}

	void ColorFeature::RegisterComponents(flecs::world& world) {
		world.component<Color>().member<FLinearColor>(VALUE)
			.add(flecs::OnInstantiate, flecs::Inherit);
		world.component<ColorSet>().add(flecs::CanToggle);

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
	}

	void ColorFeature::CreateQueries(flecs::world& world) {
		world.set(ColorPrefabQuery{ world.query_builder<Color>().with(flecs::Prefab).cached().build() });
	};

	void ColorFeature::CreateObservers(flecs::world& world) {
		world.observer<const UIScheme>("UpdateColorScheme")
			.term_at(0).singleton()
			.event(flecs::OnSet)
			.each([&world](const UIScheme& s) {
			using namespace material_color_utilities;
			auto ds = DynamicScheme(
				Hct(s.Primary),
				s.Variant,
				s.Contrast,
				s.DarkMode,
				TonalPalette(s.Primary),
				TonalPalette(s.Secondary),
				TonalPalette(s.Tertiary),
				TonalPalette(s.Neutral),
				TonalPalette(s.NeutralVariant),
				TonalPalette(s.Error));

			SetColor(world, "Background", MaterialDynamicColors::Background().GetLinear(ds));
			SetColor(world, "OnBackground", MaterialDynamicColors::OnBackground().GetLinear(ds));
			SetColor(world, "Surface", MaterialDynamicColors::Surface().GetLinear(ds));
			SetColor(world, "SurfaceDim", MaterialDynamicColors::SurfaceDim().GetLinear(ds));
			SetColor(world, "SurfaceBright", MaterialDynamicColors::SurfaceBright().GetLinear(ds));
			SetColor(world, "SurfaceContainerLowest", MaterialDynamicColors::SurfaceContainerLowest().GetLinear(ds));
			SetColor(world, "SurfaceContainerLow", MaterialDynamicColors::SurfaceContainerLow().GetLinear(ds));
			SetColor(world, "SurfaceContainer", MaterialDynamicColors::SurfaceContainer().GetLinear(ds));
			SetColor(world, "SurfaceContainerHigh", MaterialDynamicColors::SurfaceContainerHigh().GetLinear(ds));
			SetColor(world, "SurfaceContainerHighest", MaterialDynamicColors::SurfaceContainerHighest().GetLinear(ds));
			SetColor(world, "OnSurface", MaterialDynamicColors::OnSurface().GetLinear(ds));
			SetColor(world, "SurfaceVariant", MaterialDynamicColors::SurfaceVariant().GetLinear(ds));
			SetColor(world, "OnSurfaceVariant", MaterialDynamicColors::OnSurfaceVariant().GetLinear(ds));
			SetColor(world, "InverseSurface", MaterialDynamicColors::InverseSurface().GetLinear(ds));
			SetColor(world, "InverseOnSurface", MaterialDynamicColors::InverseOnSurface().GetLinear(ds));
			SetColor(world, "Outline", MaterialDynamicColors::Outline().GetLinear(ds));
			SetColor(world, "OutlineVariant", MaterialDynamicColors::OutlineVariant().GetLinear(ds));
			SetColor(world, "Shadow", MaterialDynamicColors::Shadow().GetLinear(ds));
			SetColor(world, "Scrim", MaterialDynamicColors::Scrim().GetLinear(ds));
			SetColor(world, "SurfaceTint", MaterialDynamicColors::SurfaceTint().GetLinear(ds));
			SetColor(world, "Primary", MaterialDynamicColors::Primary().GetLinear(ds));
			SetColor(world, "OnPrimary", MaterialDynamicColors::OnPrimary().GetLinear(ds));
			SetColor(world, "PrimaryContainer", MaterialDynamicColors::PrimaryContainer().GetLinear(ds));
			SetColor(world, "OnPrimaryContainer", MaterialDynamicColors::OnPrimaryContainer().GetLinear(ds));
			SetColor(world, "InversePrimary", MaterialDynamicColors::InversePrimary().GetLinear(ds));
			SetColor(world, "Secondary", MaterialDynamicColors::Secondary().GetLinear(ds));
			SetColor(world, "OnSecondary", MaterialDynamicColors::OnSecondary().GetLinear(ds));
			SetColor(world, "SecondaryContainer", MaterialDynamicColors::SecondaryContainer().GetLinear(ds));
			SetColor(world, "OnSecondaryContainer", MaterialDynamicColors::OnSecondaryContainer().GetLinear(ds));
			SetColor(world, "Tertiary", MaterialDynamicColors::Tertiary().GetLinear(ds));
			SetColor(world, "OnTertiary", MaterialDynamicColors::OnTertiary().GetLinear(ds));
			SetColor(world, "TertiaryContainer", MaterialDynamicColors::TertiaryContainer().GetLinear(ds));
			SetColor(world, "OnTertiaryContainer", MaterialDynamicColors::OnTertiaryContainer().GetLinear(ds));
			SetColor(world, "Error", MaterialDynamicColors::Error().GetLinear(ds));
			SetColor(world, "OnError", MaterialDynamicColors::OnError().GetLinear(ds));
			SetColor(world, "ErrorContainer", MaterialDynamicColors::ErrorContainer().GetLinear(ds));
			SetColor(world, "OnErrorContainer", MaterialDynamicColors::OnErrorContainer().GetLinear(ds));
			SetColor(world, "PrimaryFixed", MaterialDynamicColors::PrimaryFixed().GetLinear(ds));
			SetColor(world, "PrimaryFixedDim", MaterialDynamicColors::PrimaryFixedDim().GetLinear(ds));
			SetColor(world, "OnPrimaryFixed", MaterialDynamicColors::OnPrimaryFixed().GetLinear(ds));
			SetColor(world, "OnPrimaryFixedVariant", MaterialDynamicColors::OnPrimaryFixedVariant().GetLinear(ds));
			SetColor(world, "SecondaryFixed", MaterialDynamicColors::SecondaryFixed().GetLinear(ds));
			SetColor(world, "SecondaryFixedDim", MaterialDynamicColors::SecondaryFixedDim().GetLinear(ds));
			SetColor(world, "OnSecondaryFixed", MaterialDynamicColors::OnSecondaryFixed().GetLinear(ds));
			SetColor(world, "OnSecondaryFixedVariant", MaterialDynamicColors::OnSecondaryFixedVariant().GetLinear(ds));
			SetColor(world, "TertiaryFixed", MaterialDynamicColors::TertiaryFixed().GetLinear(ds));
			SetColor(world, "TertiaryFixedDim", MaterialDynamicColors::TertiaryFixedDim().GetLinear(ds));
			SetColor(world, "OnTertiaryFixed", MaterialDynamicColors::OnTertiaryFixed().GetLinear(ds));
			SetColor(world, "OnTertiaryFixedVariant", MaterialDynamicColors::OnTertiaryFixedVariant().GetLinear(ds)); });
	}

	void ColorFeature::CreateSystems(flecs::world& world) {
		world.system("AddColorSet")
			.with(flecs::Prefab)
			.with<Color>()
			.without<ColorSet>()
			.each([](flecs::entity e) { e.add<ColorSet>().disable<ColorSet>(); });

		world.system<const Color>("UpdateColor")
			.with(flecs::Prefab)
			.with<ColorSet>().id_flags(flecs::TOGGLE).without<ColorSet>()
			.each([&world](flecs::entity p, const Color& c) {
			p.enable<ColorSet>();
			world.each(world.pair(flecs::IsA, p), [&c](flecs::entity i) {
				if (i.has<TextBlock>() && i.has<Widget>())
					StaticCastSharedPtr<STextBlock>(i.get_mut<Widget>()->Value)->SetColorAndOpacity(c.Value); });
				});
	}
}