// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "flecs.h"
#include "UIFeature.h"
#include "WidgetFeature.h"
#include "StyleFeature.h"

namespace UI {
	struct ButtonFeature {
		static void RegisterComponents(flecs::world& world);
		static void CreateSystems(flecs::world& world);
	};

	struct Border {};
	struct Button {};

	static inline void AddButtonWidget(flecs::world world, flecs::entity entity) {
		entity.set(WidgetInstance{ SNew(SButton).ButtonStyle(&entity.get<ButtonStyle>()->Value)
			.OnClicked_Lambda(([entity]() { entity.add(Clicked); return FReply::Handled(); })) });
	}

	static inline void AddBorderWidget(flecs::entity entity) {
		entity.set(WidgetInstance{ SNew(SBorder)
			.Padding(0)
			.BorderImage(FAppStyle::Get().GetBrush("WhiteBrush")) });
	}
}