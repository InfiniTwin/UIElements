// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "flecs.h"

constexpr const char* GameViewport = "GameViewport";

namespace UIElements {
	struct Widget { TSharedPtr<SWidget> Value; };

	struct Viewport {};

	struct CompoundWidgetInstance : public SCompoundWidget {
	public:
		SLATE_BEGIN_ARGS(CompoundWidgetInstance) {}
		SLATE_END_ARGS()

		void Construct(const FArguments& InArgs) {}

		FCompoundWidgetOneChildSlot& Slot() { return ChildSlot; }
	};
	struct CompoundWidget {};

	struct Border {};

	struct Attached {};

	struct WidgetFeature {
		static void RegisterComponents(flecs::world& world);
		static void CreateSystems(flecs::world& world);
		static void Initialize(flecs::world& world);
	};
}