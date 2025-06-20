// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "flecs.h"
#include "UIFeature.h"
#include "WidgetFeature.h"
#include "StyleFeature.h"

namespace UI {
	struct ButtonFeature {
		static void RegisterComponents(flecs::world& world);
		static void CreateQueries(flecs::world& world);
		static void CreateObservers(flecs::world& world);
	};

	struct Button {};
	struct CheckBox {};
	struct Toggle {};

	struct ButtonStyle { FButtonStyle Value; };
	struct CheckBoxStyle { FCheckBoxStyle Value; };

	struct QueryButtonStylePrefab { flecs::query<ButtonStyle> Value; };
	struct QueryCheckBoxStylePrefab { flecs::query<CheckBoxStyle> Value; };

	static inline void AddButtonWidget(flecs::world world, flecs::entity entity) {
		entity.set(WidgetInstance{ SNew(SButton)
			.ButtonStyle(&entity.get<ButtonStyle>()->Value)
			.OnClicked_Lambda(([entity]() { entity.add(Clicked); return FReply::Handled(); })) });
	}

	static inline void AddCheckBoxWidget(flecs::world world, flecs::entity entity) {
		entity.set(WidgetInstance{ SNew(SCheckBox)
			.Style(&entity.get<CheckBoxStyle>()->Value) });
	}

	static inline void SetButtonStyle(flecs::entity entity) {
		auto style = FButtonStyle();
		entity.children([&style](flecs::entity brush) {
			if (brush.name().contains("Hovered"))
				style.SetHovered(GetBrush(brush));
			else {
				auto margin = ToMargin(brush.get<Padding>());
				if (brush.name().contains("Normal"))
				{
					style.SetNormal(GetBrush(brush));
					style.SetNormalPadding(margin);
				}
				else if (brush.name().contains("Pressed")) {
					style.SetPressed(GetBrush(brush));
					style.SetPressedPadding(margin);
				}
			}
			});
		entity.get_mut<ButtonStyle>()->Value = style;
	}

	static inline void SetCheckBoxStyle(flecs::entity entity) {
		auto fbs = FTextBlockStyle();
		auto style = FCheckBoxStyle();
		style.CheckBoxType = static_cast<ESlateCheckBoxType::Type>(entity.has<Toggle>());
		style.SetPadding(ToMargin(entity.get<Padding>()));

		entity.children([&style](flecs::entity brush) {
			if (brush.name().contains("BrushNormalOFF"))
				style.SetUncheckedImage(GetBrush(brush));
			else if (brush.name().contains("BrushNormalON"))
				style.SetCheckedImage(GetBrush(brush));

			else if (brush.name().contains("BrushHoveredOFF"))
				style.SetUncheckedHoveredImage(GetBrush(brush));
			else if (brush.name().contains("BrushHoveredON"))
				style.SetCheckedHoveredImage(GetBrush(brush));


			else if (brush.name().contains("BrushPressedOFF"))
				style.SetUncheckedPressedImage(GetBrush(brush));
			else if (brush.name().contains("BrushPressedON"))
				style.SetCheckedPressedImage(GetBrush(brush));

			else if (brush.name().contains("ForegroundNormal"))
				style.SetForegroundColor(brush.get<Color>()->Value);
			else if (brush.name().contains("ForegroundHovered")) {
				FLinearColor color = brush.get<Color>()->Value;
				style.SetCheckedForegroundColor(color);
				style.SetHoveredForegroundColor(color);
				style.SetCheckedHoveredForegroundColor(color);
				style.SetPressedForegroundColor(color);
				style.SetCheckedPressedForegroundColor(color);
			}
			}
		);
		entity.get_mut<CheckBoxStyle>()->Value = style;
	}
}