// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "flecs.h"

namespace UIElements {
	struct Viewport {};

	struct UIWidget {};
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

	struct HAlign { EHorizontalAlignment Value; };
	struct VAlign { EVerticalAlignment Value; };
	struct Padding { std::vector<float> Value; }; // [Left, Top, Right, Bottom]

	struct Border {};
	struct RoundBorder {};

	struct Parented {};
	struct StyleSynced {};

	struct WidgetFeature {
		static void RegisterOpaqueTypes(flecs::world& world);
		static void RegisterComponents(flecs::world& world);
		static void CreateObservers(flecs::world& world);
		static void CreateSystems(flecs::world& world);
	};

	template<typename SlotType>
	void CreateSlot(SlotType& slot, const flecs::entity child)
	{
		std::vector<float> padding = child.get<Padding>()->Value;

		slot
			.HAlign(child.get<HAlign>()->Value)
			.VAlign(child.get<VAlign>()->Value)
			.Padding(padding[0], padding[1], padding[2], padding[3])
			.AttachWidget(child.get<Widget>()->Value.ToSharedRef());
	}

	static inline void ParentToCompoundWidget(const TSharedRef<SWidget> child, const TSharedRef<SWidget> parent) {
		StaticCastSharedRef<CompoundWidgetInstance>(parent)->Slot().AttachWidget(child);
	}

	static inline void ParentToBox(const TSharedRef<SWidget> child, const TSharedRef<SWidget> parent) {
		TSharedRef<SBox> box = StaticCastSharedRef<SBox>(parent);
		box->SetHAlign(HAlign_Left);
		box->SetVAlign(VAlign_Top);
		box->SetWidthOverride(FOptionalSize());
		box->SetHeightOverride(FOptionalSize());
		box->SetContent(child);
	}

	static inline void ParentToHorizontalBox(const flecs::entity child, const TSharedRef<SWidget> parent) {
		auto slot = StaticCastSharedRef<SHorizontalBox>(parent)->AddSlot();
		CreateSlot(slot, child);
	}

	static inline void ParentToVerticalBox(const flecs::entity child, const TSharedRef<SWidget> parent) {
		auto slot = StaticCastSharedRef<SVerticalBox>(parent)->AddSlot();
		CreateSlot(slot, child);
	}

	static inline void ParentToBorder(const TSharedRef<SWidget> child, const TSharedRef<SWidget> parent) {
		StaticCastSharedRef<SBorder>(parent)->SetContent(child);
	}
}