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
		static void CreateObservers(flecs::world& world);
		static void Initialize(flecs::world& world);
	};

	struct Window {};
	struct WindowTitle {};
	struct Size { FVector2D Value; };

	struct QueryWindows { flecs::query<WidgetInstance> Value; };

	static inline void CloseWindow(flecs::entity window) {
		window.add(Closed);
		auto checkBox = window.parent();
		auto checkBoxWidget = StaticCastSharedRef<SCheckBox>(checkBox.try_get<WidgetInstance>()->Value.ToSharedRef());
		checkBoxWidget->SetIsChecked(ECheckBoxState::Unchecked);
		checkBoxWidget->SetVisibility(EVisibility::Visible);
		checkBox.add(Unchecked);
		window.remove<WidgetInstance>();
	}

	static inline void SetWindowTitle(const TSharedPtr<SWidget>& widget, const FString& text) {
		StaticCastSharedPtr<SWindow>(widget)->SetTitle(FText::FromString(text));
	}
}