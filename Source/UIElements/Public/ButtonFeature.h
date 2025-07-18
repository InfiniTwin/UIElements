// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "flecs.h"
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
	struct Radio {};

	struct ButtonStyle { FButtonStyle Value; };
	struct CheckBoxStyle { FCheckBoxStyle Value; };

	enum ButtonState {
		Hovered,
		Unhovered,
		Pressed,
		Released,
		Clicked
	};

	enum CheckBoxState {
		Unchecked,
		Checked,
		Undetermined
	};

	struct QueryButtonStylePrefab { flecs::query<ButtonStyle> Value; };
	struct QueryCheckBoxStylePrefab { flecs::query<CheckBoxStyle> Value; };

	static inline void AddButtonWidget(flecs::entity button) {
		button.set(WidgetInstance{ SNew(SButton)
			.ButtonStyle(&button.try_get<ButtonStyle>()->Value)
			.OnHovered_Lambda(([button]() { button.add(Hovered); }))
			.OnUnhovered_Lambda(([button]() { button.add(Unhovered); }))
			.OnPressed_Lambda(([button]() { button.add(Pressed); }))
			.OnReleased_Lambda(([button]() { button.add(Released); }))
			.OnClicked_Lambda(([button]() { button.add(Clicked); return FReply::Handled(); })) });
	}

	static inline void AddCheckBoxWidget(flecs::entity checkBox) {
		checkBox.set(WidgetInstance{ SNew(SCheckBox)
			.Style(&checkBox.try_get<CheckBoxStyle>()->Value)
			.OnCheckStateChanged_Lambda([checkBox](ECheckBoxState state) {
				switch (state) {
				case ECheckBoxState::Unchecked:
					checkBox.add(Unchecked);
					break;
				case ECheckBoxState::Checked:
					checkBox.add(Checked);
					break;
				case ECheckBoxState::Undetermined:
					checkBox.add(Undetermined);
					break;
				}}) });
	}

	static inline void SetButtonStyle(flecs::entity button) {
		auto style = FButtonStyle();
		button.children([&style](flecs::entity brush) {
			if (!brush.has<BrushType>()) return;
			if (brush.name().contains("BrushHovered"))
				style.SetHovered(GetBrush(brush));
			else {
				auto margin = brush.try_get<Padding>()->Value;
				if (brush.name().contains("BrushNormal")) {
					style.SetNormal(GetBrush(brush));
					style.SetNormalPadding(margin);
				} else if (brush.name().contains("BrushPressed")) {
					style.SetPressed(GetBrush(brush));
					style.SetPressedPadding(margin);
				}
			}
		});
		button.try_get_mut<ButtonStyle>()->Value = style;
	}

	static inline void SetCheckBoxStyle(flecs::entity checkBox) {
		auto style = FCheckBoxStyle();
		style.CheckBoxType = static_cast<ESlateCheckBoxType::Type>(checkBox.has<Toggle>());
		style.SetPadding(checkBox.try_get<Padding>()->Value);

		checkBox.children([&style](flecs::entity brush) {
			if (!brush.has<BrushType>()) return;
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
				style.SetForegroundColor(brush.try_get<Color>()->Value);
			else if (brush.name().contains("ForegroundHovered")) {
				FLinearColor color = brush.try_get<Color>()->Value;
				style.SetCheckedForegroundColor(color);
				style.SetHoveredForegroundColor(color);
				style.SetCheckedHoveredForegroundColor(color);
				style.SetPressedForegroundColor(color);
				style.SetCheckedPressedForegroundColor(color);
			}
		});
		checkBox.try_get_mut<CheckBoxStyle>()->Value = style;
	}
}