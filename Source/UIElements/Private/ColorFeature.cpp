﻿// Fill out your copyright notice in the Description page of Project Settings.

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
		using namespace ECS;
		world.component<Color>().member<FLinearColor>(VALUE)
			.add(flecs::OnInstantiate, flecs::Inherit);
		world.component<ColorSynced>().add(flecs::CanToggle);

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
		world.set(ColorPrefabQuery{ world.query_builder<Color>("ColorPrefabQuery").with(flecs::Prefab).cached().build()});
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

			SetPrefabColor(world, "Background", MaterialDynamicColors::Background().GetLinear(ds));
			SetPrefabColor(world, "OnBackground", MaterialDynamicColors::OnBackground().GetLinear(ds));
			SetPrefabColor(world, "Surface", MaterialDynamicColors::Surface().GetLinear(ds));
			SetPrefabColor(world, "SurfaceDim", MaterialDynamicColors::SurfaceDim().GetLinear(ds));
			SetPrefabColor(world, "SurfaceBright", MaterialDynamicColors::SurfaceBright().GetLinear(ds));
			SetPrefabColor(world, "SurfaceContainerLowest", MaterialDynamicColors::SurfaceContainerLowest().GetLinear(ds));
			SetPrefabColor(world, "SurfaceContainerLow", MaterialDynamicColors::SurfaceContainerLow().GetLinear(ds));
			SetPrefabColor(world, "SurfaceContainer", MaterialDynamicColors::SurfaceContainer().GetLinear(ds));
			SetPrefabColor(world, "SurfaceContainerHigh", MaterialDynamicColors::SurfaceContainerHigh().GetLinear(ds));
			SetPrefabColor(world, "SurfaceContainerHighest", MaterialDynamicColors::SurfaceContainerHighest().GetLinear(ds));
			SetPrefabColor(world, "OnSurface", MaterialDynamicColors::OnSurface().GetLinear(ds));
			SetPrefabColor(world, "SurfaceVariant", MaterialDynamicColors::SurfaceVariant().GetLinear(ds));
			SetPrefabColor(world, "OnSurfaceVariant", MaterialDynamicColors::OnSurfaceVariant().GetLinear(ds));
			SetPrefabColor(world, "InverseSurface", MaterialDynamicColors::InverseSurface().GetLinear(ds));
			SetPrefabColor(world, "InverseOnSurface", MaterialDynamicColors::InverseOnSurface().GetLinear(ds));
			SetPrefabColor(world, "Outline", MaterialDynamicColors::Outline().GetLinear(ds));
			SetPrefabColor(world, "OutlineVariant", MaterialDynamicColors::OutlineVariant().GetLinear(ds));
			SetPrefabColor(world, "Shadow", MaterialDynamicColors::Shadow().GetLinear(ds));
			SetPrefabColor(world, "Scrim", MaterialDynamicColors::Scrim().GetLinear(ds));
			SetPrefabColor(world, "SurfaceTint", MaterialDynamicColors::SurfaceTint().GetLinear(ds));
			SetPrefabColor(world, "Primary", MaterialDynamicColors::Primary().GetLinear(ds));
			SetPrefabColor(world, "OnPrimary", MaterialDynamicColors::OnPrimary().GetLinear(ds));
			SetPrefabColor(world, "PrimaryContainer", MaterialDynamicColors::PrimaryContainer().GetLinear(ds));
			SetPrefabColor(world, "OnPrimaryContainer", MaterialDynamicColors::OnPrimaryContainer().GetLinear(ds));
			SetPrefabColor(world, "InversePrimary", MaterialDynamicColors::InversePrimary().GetLinear(ds));
			SetPrefabColor(world, "Secondary", MaterialDynamicColors::Secondary().GetLinear(ds));
			SetPrefabColor(world, "OnSecondary", MaterialDynamicColors::OnSecondary().GetLinear(ds));
			SetPrefabColor(world, "SecondaryContainer", MaterialDynamicColors::SecondaryContainer().GetLinear(ds));
			SetPrefabColor(world, "OnSecondaryContainer", MaterialDynamicColors::OnSecondaryContainer().GetLinear(ds));
			SetPrefabColor(world, "Tertiary", MaterialDynamicColors::Tertiary().GetLinear(ds));
			SetPrefabColor(world, "OnTertiary", MaterialDynamicColors::OnTertiary().GetLinear(ds));
			SetPrefabColor(world, "TertiaryContainer", MaterialDynamicColors::TertiaryContainer().GetLinear(ds));
			SetPrefabColor(world, "OnTertiaryContainer", MaterialDynamicColors::OnTertiaryContainer().GetLinear(ds));
			SetPrefabColor(world, "Error", MaterialDynamicColors::Error().GetLinear(ds));
			SetPrefabColor(world, "OnError", MaterialDynamicColors::OnError().GetLinear(ds));
			SetPrefabColor(world, "ErrorContainer", MaterialDynamicColors::ErrorContainer().GetLinear(ds));
			SetPrefabColor(world, "OnErrorContainer", MaterialDynamicColors::OnErrorContainer().GetLinear(ds));
			SetPrefabColor(world, "PrimaryFixed", MaterialDynamicColors::PrimaryFixed().GetLinear(ds));
			SetPrefabColor(world, "PrimaryFixedDim", MaterialDynamicColors::PrimaryFixedDim().GetLinear(ds));
			SetPrefabColor(world, "OnPrimaryFixed", MaterialDynamicColors::OnPrimaryFixed().GetLinear(ds));
			SetPrefabColor(world, "OnPrimaryFixedVariant", MaterialDynamicColors::OnPrimaryFixedVariant().GetLinear(ds));
			SetPrefabColor(world, "SecondaryFixed", MaterialDynamicColors::SecondaryFixed().GetLinear(ds));
			SetPrefabColor(world, "SecondaryFixedDim", MaterialDynamicColors::SecondaryFixedDim().GetLinear(ds));
			SetPrefabColor(world, "OnSecondaryFixed", MaterialDynamicColors::OnSecondaryFixed().GetLinear(ds));
			SetPrefabColor(world, "OnSecondaryFixedVariant", MaterialDynamicColors::OnSecondaryFixedVariant().GetLinear(ds));
			SetPrefabColor(world, "TertiaryFixed", MaterialDynamicColors::TertiaryFixed().GetLinear(ds));
			SetPrefabColor(world, "TertiaryFixedDim", MaterialDynamicColors::TertiaryFixedDim().GetLinear(ds));
			SetPrefabColor(world, "OnTertiaryFixed", MaterialDynamicColors::OnTertiaryFixed().GetLinear(ds));
			SetPrefabColor(world, "OnTertiaryFixedVariant", MaterialDynamicColors::OnTertiaryFixedVariant().GetLinear(ds)); });
	}

	void ColorFeature::CreateSystems(flecs::world& world) {
		world.system("AddColorSynced")
			.with<Color>()
			.without<ColorSynced>()
			.each([](flecs::entity e) { e.add<ColorSynced>().disable<ColorSynced>(); });

		world.system<const Color, const Widget>("UpdateTextBlockColor")
			.with<TextBlock>()
			.with<ColorSynced>().id_flags(flecs::TOGGLE).without<ColorSynced>()
			.each([&world](flecs::entity e, const Color& c, const Widget& w) {
			e.enable<ColorSynced>();
			StaticCastSharedPtr<STextBlock>(w.Value)->SetColorAndOpacity(c.Value); });
	}

	void UIElements::SetPrefabColor(flecs::world& world, const FString name, const FLinearColor c) {
		auto colorName = TEXT("Color") + name;
		FTCHARToUTF8 cn(*colorName);
		world.get<ColorPrefabQuery>()->Value.run([&world, &cn, &c](flecs::iter& it) {
			while (it.next())
				for (auto i : it)
				{
					auto p = it.entity(i);
					if (std::strcmp(p.name(), cn.Get()) == 0)
					{
						p.set<Color>({ c });
						world.each(world.pair(flecs::IsA, p), [](flecs::entity i) {
							i.disable<ColorSynced>();
							});
						return;
					}
				}
			});
	}
}