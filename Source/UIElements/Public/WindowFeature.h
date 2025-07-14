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
	struct WindowStyle { FWindowStyle Value; };

	struct QueryWindows { flecs::query<WidgetInstance> Value; };
	struct QueryWindowStylePrefab { flecs::query<WindowStyle> Value; };

	static inline void SetWindowStyle(flecs::entity window) {
		auto style = FWindowStyle();
		window.children([&style](flecs::entity brush) {
			if (!brush.has<BrushType>()) return;
			if (brush.name().contains("BackgroundBrush"))
				style.SetBackgroundBrush(GetBrush(brush));
		});
		window.try_get_mut<WindowStyle>()->Value = style;
	}

	static inline void CloseWindow(flecs::entity window) {
		window.add(Closed);
		auto checkBox = window.parent();
		auto checkBoxWidget = StaticCastSharedRef<SCheckBox>(checkBox.try_get<WidgetInstance>()->Value.ToSharedRef());
		checkBoxWidget->SetIsChecked(ECheckBoxState::Unchecked);
		checkBoxWidget->SetVisibility(EVisibility::Visible);
		checkBox.add(Unchecked);
		window.remove<WidgetInstance>();
	}

	static inline void OpenWindow(flecs::entity window) {
		TSharedRef<SWindow> widget = SNew(SWindow)
			.Style(&window.try_get<WindowStyle>()->Value);

		widget->SetOnWindowClosed(FOnWindowClosed::CreateLambda([window](const TSharedRef<SWindow>& closedWindow) {
			CloseWindow(window);
		}));

		window.set(WidgetInstance{ widget });

		FSlateApplication::Get().AddWindow(widget);

		StaticCastSharedRef<SCheckBox>(window.parent().try_get<WidgetInstance>()->Value.ToSharedRef())
			->SetVisibility(EVisibility::HitTestInvisible);
	}

	static inline void SetWindowTitle(const TSharedPtr<SWidget>& widget, const FString& text) {
		StaticCastSharedPtr<SWindow>(widget)->SetTitle(FText::FromString(text));
	}
}