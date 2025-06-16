// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "flecs.h"
#include "ECS.h"
#include "UIFeature.h"
#include "ColorFeature.h"
#include "WidgetFeature.h"

namespace UI {
	struct StyleFeature {
		static void RegisterComponents(flecs::world& world);
		static void CreateObservers(flecs::world& world);
		static void CreateSystems(flecs::world& world);
		static void Initialize(flecs::world& world);
	};

	struct StyleSets { TSharedPtr<FSlateStyleSet> Value; };
	struct WidgetStyle { FSlateWidgetStyle Value; };

	struct Brush { FSlateBrush Value; };
	struct BrushType { int Value; };

	struct Width { float Value; };
	struct FixedRadius {};
	struct Radii { float TopLeft, TopRight, BottomRight, BottomLeft; };

	static inline FSlateBrush* ToSlateBrush(flecs::entity brush) {
		FSlateBrush* slateBrush = new FSlateBrush();
		slateBrush->DrawAs = static_cast<ESlateBrushDrawType::Type>(brush.get<BrushType>()->Value);
		slateBrush->TintColor = brush.get<Color>()->Value;
		if (slateBrush->GetDrawType() == ESlateBrushDrawType::Type::RoundedBox) {
			if (auto outline = ECS::FirstChild(brush)) {
				slateBrush->OutlineSettings.Color = outline.get<Color>()->Value;
				slateBrush->OutlineSettings.RoundingType = outline.has<FixedRadius>() ?
					ESlateBrushRoundingType::FixedRadius : ESlateBrushRoundingType::HalfHeightRadius;
				slateBrush->OutlineSettings.Width = outline.get<Width>()->Value;
				auto radii = outline.get<Radii>();
				slateBrush->OutlineSettings.CornerRadii = FVector4(
					radii->TopLeft, radii->TopRight, radii->BottomRight, radii->BottomLeft);
			}
		}
		return slateBrush;
	}

	static inline FSlateWidgetStyle AddButtonStyle(flecs::entity widgetStyle) {
		auto buttonStyle = FButtonStyle();
		widgetStyle.children([&buttonStyle](flecs::entity brush) {
			if (brush.name().contains("Normal"))
			{
				buttonStyle.SetNormal(*ToSlateBrush(brush));
				auto padding = brush.get<Padding>();
				buttonStyle.SetNormalPadding(FMargin((padding->Left, padding->Top, padding->Right, padding->Bottom)));
			}
			if (brush.name().contains("Hovered"))
				buttonStyle.SetHovered(*ToSlateBrush(brush));
			if (brush.name().contains("Pressed")) {
				buttonStyle.SetPressed(*ToSlateBrush(brush));
				auto padding = brush.get<Padding>();
				buttonStyle.SetPressedPadding(FMargin((padding->Left, padding->Top, padding->Right, padding->Bottom)));
			}
					});
		return buttonStyle;
	}
}