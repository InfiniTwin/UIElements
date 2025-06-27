// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "flecs.h"
#include "ECS.h"
#include "ColorFeature.h"

namespace UI {
	struct StyleFeature {
		static void RegisterComponents(flecs::world& world);
		static void CreateQueries(flecs::world& world);
		static void CreateObservers(flecs::world& world);
	};

	struct Brush { FSlateBrush Value; };
	struct BrushType { int Value; };

	struct Width { float Value; };
	struct FixedRadius {};
	struct Radii { float TopLeft, TopRight, BottomRight, BottomLeft; };

	struct QueryBrushPrefab { flecs::query<Brush> Value; };

	static inline FSlateBrush GetBrush(flecs::entity brush) {
		FSlateBrush slateBrush;
		slateBrush.DrawAs = static_cast<ESlateBrushDrawType::Type>(brush.get<BrushType>()->Value);
		slateBrush.TintColor = brush.get<Color>()->Value;
		if (slateBrush.GetDrawType() == ESlateBrushDrawType::Type::RoundedBox) {
			if (auto outline = ECS::FirstChild(brush)) {
				slateBrush.OutlineSettings.Color = outline.get<Color>()->Value;
				if (!outline.has<FixedRadius>())
					slateBrush.OutlineSettings.RoundingType = ESlateBrushRoundingType::HalfHeightRadius;
				else {
					slateBrush.OutlineSettings.RoundingType = ESlateBrushRoundingType::FixedRadius;
					auto radii = outline.get<Radii>();
					slateBrush.OutlineSettings.CornerRadii = FVector4(
						radii->TopLeft, radii->TopRight, radii->BottomRight, radii->BottomLeft);
				}
				slateBrush.OutlineSettings.Width = outline.get<Width>()->Value;
			}
		}
		return slateBrush;
	}

	static inline void SetBrush(flecs::entity brush) {
		brush.get_mut<Brush>()->Value = GetBrush(brush);
	}
}