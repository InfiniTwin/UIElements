// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "flecs.h"
#include "WidgetFeature.h"
#include "UIFeature.h"

namespace UI {
	struct ButtonFeature {
		static void RegisterOpaqueTypes(flecs::world& world);
		static void RegisterComponents(flecs::world& world);
		static void CreateSystems(flecs::world& world);
	};

	struct Button {};

	//struct BrushType { ESlateBrushDrawType Value; };

	struct ButtonStyle {
		FString NormalBrush;
		FString HoveredBrush;
		FString PressedBrush;
	};
	struct Brush {};
	struct BrushType { ESlateBrushDrawType::Type Value; };

	static inline void SetRoundedBoxBrush(FSlateRoundedBoxBrush& brush, const flecs::entity entity) {
	}

	static inline void AddButtonWidget(flecs::entity entity) {
		entity.set(WidgetInstance{ SNew(SButton)
			.OnHovered_Lambda(([entity]() { entity.add(Hovered); }))
			.OnUnhovered_Lambda(([entity]() { entity.add(Normal); }))
			.OnClicked_Lambda(([entity]() { entity.add(Clicked); return FReply::Handled(); }))
			.OnPressed_Lambda(([entity]() { entity.add(Pressed); }))
			.OnReleased_Lambda(([entity]() { entity.add(Released); }))
			});
		entity.add(Normal);
	}
}