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

	struct Order { int Value; };

	struct WidgetFeature {
		static void RegisterOpaqueTypes(flecs::world& world);
		static void RegisterComponents(flecs::world& world);
		static void CreateObservers(flecs::world& world);
		static void CreateSystems(flecs::world& world);
	};

	static inline FMargin GetMargin(const std::vector<float> padding) {
		return FMargin(padding[0], padding[1], padding[2], padding[3]);
	}

	template<typename SlotType>
	void AttachSlot(SlotType& slot, const flecs::entity child)
	{
		slot
			.VAlign(child.get<VAlign>()->Value)
			.HAlign(child.get<HAlign>()->Value)
			.Padding(GetMargin(child.get<Padding>()->Value))
			.AttachWidget(child.get<Widget>()->Value.ToSharedRef());
	}

	static inline void AttachToCompoundWidget(const TSharedRef<SWidget> child, const TSharedRef<SWidget> parent) {
		StaticCastSharedRef<CompoundWidgetInstance>(parent)->Slot().AttachWidget(child);
	}

	static inline void AttachToBox(const flecs::entity child, const TSharedRef<SWidget> parent) {
		TSharedRef<SBox> box = StaticCastSharedRef<SBox>(parent);
		box->SetVAlign(child.get<VAlign>()->Value);
		box->SetHAlign(child.get<HAlign>()->Value);
		box->SetWidthOverride(FOptionalSize());
		box->SetHeightOverride(FOptionalSize());
		box->SetPadding(GetMargin(child.get<Padding>()->Value));
		box->SetContent(child.get<Widget>()->Value.ToSharedRef());
	}

	static inline void AttachToHorizontalBox(const flecs::entity child, const TSharedRef<SWidget> parent) {
		auto slot = StaticCastSharedRef<SHorizontalBox>(parent)->AddSlot();
		slot.AutoWidth();
		AttachSlot(slot, child);
	}

	static inline void AttachToVerticalBox(const flecs::entity child, const TSharedRef<SWidget> parent) {
		auto slot = StaticCastSharedRef<SVerticalBox>(parent)->AddSlot();
		slot.AutoHeight();
		AttachSlot(slot, child);
	}

	static inline void AttachToBorder(const flecs::entity child, const TSharedRef<SWidget> parent) {
		StaticCastSharedRef<SBorder>(parent)->SetContent(child.get<Widget>()->Value.ToSharedRef());
	}

	int SortOrder(flecs::entity_t e1, const Order* o1, flecs::entity_t e2, const Order* o2) {
		(void)e1;
		(void)e2;
		return (o1->Value > o2->Value) - (o1->Value < o2->Value);
	}
}