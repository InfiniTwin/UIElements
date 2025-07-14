// Fill out your copyright notice in the Description page of Project Settings.

#include "ColorFeature.h"
#include "ECS.h"
#include "UIUtils.h"
#include "dynamiccolor/dynamic_scheme.h"
#include "dynamiccolor/material_dynamic_colors.h"
#include "palettes/tones.h"
#include "WidgetFeature.h"
#include "TypographyFeature.h"
#include "ButtonFeature.h"

namespace UI {
	void ColorFeature::RegisterOpaqueTypes(flecs::world& world) {
		// MCU Variant <=> int
		using namespace material_color_utilities;
		world.component<Variant>()
			.opaque(flecs::I32)
			.serialize([](const flecs::serializer* s, const Variant* data) {
			int value = static_cast<int>(*data);
			return s->value(flecs::I32, &value);
		})
			.assign_int([](Variant* data, int64_t value) {
			if (value >= static_cast<int64_t>(Variant::kMonochrome) &&
				value <= static_cast<int64_t>(Variant::kFruitSalad)) {
				*data = static_cast<Variant>(value);
			} else {
				*data = Variant::kMonochrome;
			}
		});

		// Argb <=> Hex (as string)
		world.component<Argb>()
			.opaque(flecs::String)
			.serialize([](const flecs::serializer* s, const Argb* data) {
			FString HexStr = UIUtils::HexFromArgb(*data);
			std::string Utf8 = TCHAR_TO_UTF8(*HexStr);
			const char* CStr = Utf8.c_str();
			return s->value(flecs::String, &CStr);
		})
			.assign_string([](Argb* data, const char* value) {
			FString Hex = UTF8_TO_TCHAR(value);
			*data = UIUtils::ArgbFromHex(Hex);
		});
	}

	void ColorFeature::RegisterComponents(flecs::world& world) {
		using namespace ECS;
		world.component<Color>().member<FLinearColor>(VALUE).add(flecs::OnInstantiate, flecs::Inherit);

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
		world.component<QueryColorPrefab>();
		world.set(QueryColorPrefab{
			world.query_builder<Color>(COMPONENT(QueryColorPrefab))
			.with(flecs::Prefab)
			.cached().build() });
	};

	void ColorFeature::CreateObservers(flecs::world& world) {
		world.observer<const UIScheme>("SetPrefabColor")
			.term_at(0).singleton()
			.event(flecs::OnSet)
			.each([&world](const UIScheme& scheme) {
			using namespace material_color_utilities;
			auto ds = DynamicScheme(
				Hct(scheme.Primary),
				scheme.Variant,
				scheme.Contrast,
				scheme.DarkMode,
				TonalPalette(scheme.Primary),
				TonalPalette(scheme.Secondary),
				TonalPalette(scheme.Tertiary),
				TonalPalette(scheme.Neutral),
				TonalPalette(scheme.NeutralVariant),
				TonalPalette(scheme.Error));

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
			SetPrefabColor(world, "OnTertiaryFixedVariant", MaterialDynamicColors::OnTertiaryFixedVariant().GetLinear(ds));

			world.try_get<QueryBrushPrefab>()->Value
				.each([&world](flecs::entity brush, Brush) {
				if (brush.owns<Brush>())
					brush.try_get_mut<Brush>()->Value = GetBrush(brush);
			});

			world.try_get<QueryButtonStylePrefab>()->Value
				.each([](flecs::entity style, ButtonStyle) {
				SetButtonStyle(style);
			});

			world.try_get<QueryCheckBoxStylePrefab>()->Value
				.each([](flecs::entity style, CheckBoxStyle) {
				SetCheckBoxStyle(style);
			});
		});

		world.observer<const Color>("SetInstanceWidgetColor")
			.with(flecs::Prefab)
			.event(flecs::OnSet)
			.each([&world](flecs::entity prefab, const Color& color) {
			TArray<flecs::entity> instances;
			ECS::GetInstances(world, prefab, instances);
			for (flecs::entity instance : instances)
				SetWidgetColor(world, instance, color.Value);
		}
			);

		world.observer<const Color>("SetWidgetColor")
			.event(flecs::OnSet)
			.each([&world](flecs::entity entity, const Color& color) {
			SetWidgetColor(world, entity, color.Value);
		}
			);
	}

	void SetPrefabColor(flecs::world& world, const FString name, const FLinearColor c) {
		auto colorName = TEXT("Color") + name;
		FTCHARToUTF8 cn(*colorName);
		world.try_get<QueryColorPrefab>()->Value.run([&cn, &c](flecs::iter& it) {
			while (it.next())
				for (auto i : it) {
					auto prefab = it.entity(i);
					if (std::strcmp(prefab.name(), cn.Get()) == 0) {
						prefab.set<Color>({ c });
						return;
					}
				}
		});
	}

	void SetWidgetColor(flecs::world& world, flecs::entity entity, const FLinearColor color) {
		if (!entity.has<WidgetInstance>())
			return;
		TSharedPtr<SWidget> widget = entity.try_get_mut<WidgetInstance>()->Value;

		if (entity.has<TextBlock>())
			StaticCastSharedPtr<STextBlock>(widget)->SetColorAndOpacity(color);
	}
}