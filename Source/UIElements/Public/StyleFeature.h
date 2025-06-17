// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "flecs.h"
#include "ECS.h"
#include "UIFeature.h"
#include "ColorFeature.h"
#include "WidgetFeature.h"
#include "ButtonFeature.h"

namespace UI {
	struct StyleFeature {
		static void RegisterComponents(flecs::world& world);
		static void CreateQueries(flecs::world& world);
		static void CreateObservers(flecs::world& world);
	};

	struct ButtonStyle { FButtonStyle Value; };

	struct Brush { FSlateBrush Value; };
	struct BrushType { int Value; };

	struct Width { float Value; };
	struct FixedRadius {};
	struct Radii { float TopLeft, TopRight, BottomRight, BottomLeft; };

	struct QueryButtonStylePrefab { flecs::query<ButtonStyle> Value; };

	static inline FSlateBrush ToSlateBrush(flecs::entity brush) {
		FSlateBrush slateBrush;
		slateBrush.DrawAs = static_cast<ESlateBrushDrawType::Type>(brush.get<BrushType>()->Value);
		slateBrush.TintColor = brush.get<Color>()->Value;
		if (slateBrush.GetDrawType() == ESlateBrushDrawType::Type::RoundedBox) {
			if (auto outline = ECS::FirstChild(brush)) {
				slateBrush.OutlineSettings.Color = outline.get<Color>()->Value;
				slateBrush.OutlineSettings.RoundingType = outline.has<FixedRadius>() ?
					ESlateBrushRoundingType::FixedRadius : ESlateBrushRoundingType::HalfHeightRadius;
				slateBrush.OutlineSettings.Width = outline.get<Width>()->Value;
				auto radii = outline.get<Radii>();
				slateBrush.OutlineSettings.CornerRadii = FVector4(
					radii->TopLeft, radii->TopRight, radii->BottomRight, radii->BottomLeft);
			}
		}
		return slateBrush;
	}

	static inline FButtonStyle AddButtonStyle(flecs::entity widgetStyle) {
		auto buttonStyle = FButtonStyle();
		widgetStyle.children([&buttonStyle](flecs::entity brush) {
			if (brush.name().contains("Hovered"))
				buttonStyle.SetHovered(ToSlateBrush(brush));
			else {
				auto padding = brush.get<Padding>();
				auto margin = FMargin((padding->Left, padding->Top, padding->Right, padding->Bottom));
				if (brush.name().contains("Normal"))
				{
					buttonStyle.SetNormal(ToSlateBrush(brush));
					buttonStyle.SetNormalPadding(margin);
				}
				if (brush.name().contains("Pressed")) {
					buttonStyle.SetPressed(ToSlateBrush(brush));
					buttonStyle.SetPressedPadding(margin);
				}
			}
			});
		return buttonStyle;
	}
}