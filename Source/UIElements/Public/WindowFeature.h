// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "flecs.h"
#include "WidgetFeature.h"

namespace UI {
	struct WindowFeature {
		static void CreateComponents(flecs::world& world);
		static void CreateQueries(flecs::world& world);
		static void CreateObservers(flecs::world& world);
		static void Initialize(flecs::world& world);
	};

	struct Window {};
	struct WindowTitle {};
	struct Size { FVector2D Value; };
	struct Position { FVector2D Value; };

	struct QueryWindows { flecs::query<WidgetInstance> Value; };

	static inline void SetWindowTitle(const TSharedPtr<SWidget>& widget, const FString& text) {
		StaticCastSharedPtr<SWindow>(widget)->SetTitle(FText::FromString(text));
	}
}