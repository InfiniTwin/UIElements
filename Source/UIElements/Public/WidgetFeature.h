// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "flecs.h"

namespace UIElements {
	struct WidgetFeature {
		static void RegisterComponents(flecs::world& world);
		static void RegisterSystems(flecs::world& world);
		static void Initialize(flecs::world& world);
	};

	struct CompoundWidget : public SCompoundWidget {
	public:
		SLATE_BEGIN_ARGS(CompoundWidget) {}
		SLATE_END_ARGS()

		void Construct(const FArguments& InArgs) {}

		FCompoundWidgetOneChildSlot& Slot() { return ChildSlot; }
	};

	struct Widget { TSharedPtr<CompoundWidget> Value; };
}