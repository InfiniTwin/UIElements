// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "flecs.h"

namespace UIElements {
	struct Viewport {};

	struct Widget { TSharedPtr<SWidget> Value; };

	struct CompoundWidgetInstance : public SCompoundWidget {
	public:
		SLATE_BEGIN_ARGS(CompoundWidgetInstance) {}
		SLATE_END_ARGS()

		void Construct(const FArguments& InArgs) {}

		FCompoundWidgetOneChildSlot& Slot() { return ChildSlot; }
	};
	struct CompoundWidget {};

	struct Box {};
	struct HBox {};
	struct VBox {};
	struct Border {};
	struct RoundBorder {};

	struct Parented {};
	struct StyleSynced {};

	struct WidgetFeature {
		static void RegisterComponents(flecs::world& world);
		static void CreateSystems(flecs::world& world);
	};

	static inline void ParentToBox(const TSharedRef<SWidget> childWidget, const flecs::entity parent) {
		TSharedRef<SBox> box = StaticCastSharedRef<SBox>(parent.get_mut<Widget>()->Value.ToSharedRef());
		box->SetHAlign(HAlign_Center);
		box->SetVAlign(VAlign_Center);
		box->SetWidthOverride(FOptionalSize());
		box->SetHeightOverride(FOptionalSize());
		box->SetContent(childWidget);
	}

	static inline void ParentToHorizontalBox(const TSharedRef<SWidget> childWidget, const flecs::entity parent) {
		StaticCastSharedRef<SHorizontalBox>(parent.get_mut<Widget>()->Value.ToSharedRef())
			->AddSlot().AutoWidth().HAlign(HAlign_Center).VAlign(VAlign_Center)
			[childWidget];
	}

	static inline void ParentToVerticalBox(const TSharedRef<SWidget> childWidget, const flecs::entity parent) {
		StaticCastSharedRef<SVerticalBox>(parent.get_mut<Widget>()->Value.ToSharedRef())
			->AddSlot().AutoHeight().HAlign(HAlign_Center).VAlign(VAlign_Center)
			[childWidget];
	}

	static inline void ParentToBorder(const TSharedRef<SWidget> childWidget, const flecs::entity parent) {
		StaticCastSharedRef<SBorder>(parent.get_mut<Widget>()->Value.ToSharedRef())
			->SetContent(childWidget);
	}
}