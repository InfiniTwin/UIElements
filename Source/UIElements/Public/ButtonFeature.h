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

	struct Border {};
	struct Button {};
	struct Toggle {};

	struct ButtonStyle { FButtonStyle Value; };

	struct QueryButtonStylePrefab { flecs::query<ButtonStyle> Value; };

	static inline void AddButtonWidget(flecs::world world, flecs::entity entity) {
		entity.set(WidgetInstance{ SNew(SButton).ButtonStyle(&entity.get<ButtonStyle>()->Value)
			.OnClicked_Lambda(([entity]() { entity.add(Clicked); return FReply::Handled(); })) });
	}

	static inline void AddBorderWidget(flecs::entity entity) {
		entity.set(WidgetInstance{ SNew(SBorder)
			.Padding(0)
			.BorderImage(FAppStyle::Get().GetBrush("WhiteBrush")) });
	}

	static inline FButtonStyle GetButtonStyle(flecs::entity widgetStyle) {
		auto buttonStyle = FButtonStyle();
		widgetStyle.children([&buttonStyle](flecs::entity brush) {
			if (brush.name().contains("Hovered"))
				buttonStyle.SetHovered(GetBrush(brush));
			else {
				auto padding = brush.get<Padding>();
				auto margin = FMargin((padding->Left, padding->Top, padding->Right, padding->Bottom));
				if (brush.name().contains("Normal"))
				{
					buttonStyle.SetNormal(GetBrush(brush));
					buttonStyle.SetNormalPadding(margin);
				}
				if (brush.name().contains("Pressed")) {
					buttonStyle.SetPressed(GetBrush(brush));
					buttonStyle.SetPressedPadding(margin);
				}
			}
			});
		return buttonStyle;
	}
}