// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "flecs.h"
#include "UIFeature.h"
#include "WidgetFeature.h"
#include "StyleFeature.h"

namespace UI {
	struct WindowFeature {
		static void RegisterComponents(flecs::world& world);
		static void CreateQueries(flecs::world& world);
		static void CreateSystems(flecs::world& world);
	};

	struct Window {};

	static inline void OpenWindow(flecs::entity window) {
		auto widget =
			SNew(SWindow)
			//.Title
			//.Content
			//.CreateTitleBar
			//.Style
			;
		widget->SetOnWindowClosed(FOnWindowClosed::CreateLambda([window](const TSharedRef<SWindow>& closedWindow) {
			window.add(Closed);
			}));
		window.set(WidgetInstance{ widget });

		FSlateApplication::Get().AddWindow(widget);
	}
}