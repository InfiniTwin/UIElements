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
		static void Initialize(flecs::world& world);
	};

	struct Window {};
	struct WindowTitle {};
	struct WindowStyle { FWindowStyle Value; };

	struct QueryWindows { flecs::query<WidgetInstance> Value; };

	static inline void CloseWindow(flecs::entity window) {
		window.add(Closed);
		auto checkBox = window.parent();
		auto checkBoxWidget = StaticCastSharedRef<SCheckBox>(checkBox.get<WidgetInstance>().Value.ToSharedRef());
		checkBoxWidget->SetIsChecked(ECheckBoxState::Unchecked);
		checkBoxWidget->SetVisibility(EVisibility::Visible);
		checkBox.add(Unchecked);
		window.remove<WidgetInstance>();
	}

	static inline void OpenWindow(flecs::entity window) {
		StaticCastSharedRef<SCheckBox>(window.parent().get<WidgetInstance>().Value.ToSharedRef())
			->SetVisibility(EVisibility::HitTestInvisible);

		auto widget =
			SNew(SWindow)
			//.Content
			//.CreateTitleBar
			//.Style
			;
		widget->SetOnWindowClosed(FOnWindowClosed::CreateLambda([window](const TSharedRef<SWindow>& closedWindow) {
			CloseWindow(window);
		}));
		window.set(WidgetInstance{ widget });

		FSlateApplication::Get().AddWindow(widget);
	}

	static inline void SetWindowTitle(const TSharedPtr<SWidget>& widget, const FString& text) {
		StaticCastSharedPtr<SWindow>(widget)->SetTitle(FText::FromString(text));
	}
}