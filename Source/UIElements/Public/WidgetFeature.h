// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "flecs.h"
#include "Widgets/DeclarativeSyntaxSupport.h"

namespace UIElements {
	struct Widget { TSharedPtr<SWidget> Value; };

	struct CompoundWidgetInstance : public SCompoundWidget {
	public:
		SLATE_BEGIN_ARGS(CompoundWidgetInstance) {}
		SLATE_END_ARGS()

		void Construct(const FArguments& InArgs) {}

		FCompoundWidgetOneChildSlot& Slot() { return ChildSlot; }
	};

	struct CompoundWidget { TSharedPtr<CompoundWidgetInstance> Value; };
	
	struct Viewport {};

	struct WidgetFeature {
		static void RegisterComponents(flecs::world& world);
		static void RegisterObservers(flecs::world& world);
		static void Initialize(flecs::world& world);
	};
}