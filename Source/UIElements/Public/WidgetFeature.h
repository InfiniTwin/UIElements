// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "flecs.h"
#include "UIFeature.h"

namespace UI {
	struct WidgetFeature {
		static void RegisterOpaqueTypes(flecs::world& world);
		static void RegisterComponents(flecs::world& world);
		static void CreateObservers(flecs::world& world);
		static void CreateSystems(flecs::world& world);
	};

	struct Viewport {};

	struct Widget {};
	struct WidgetInstance { TSharedPtr<SWidget> Value; };

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
	struct Padding { float Left, Top, Right, Bottom; };

	struct RoundedBoxBrush {};

	struct Attached {};
	struct StyleSynced {};

	struct Order { int Value; };
	
	template<typename SlotType>
	void AttachSlot(SlotType& slot, const flecs::entity child)
	{
		auto padding = child.get<Padding>();
		slot
			.VAlign(child.get<VAlign>()->Value)
			.HAlign(child.get<HAlign>()->Value)
			.Padding(padding->Left, padding->Top, padding->Right, padding->Bottom)
			.AttachWidget(child.get<WidgetInstance>()->Value.ToSharedRef());
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
		auto padding = child.get<Padding>();
		box->SetPadding(FMargin(padding->Left, padding->Top, padding->Right, padding->Bottom));
		box->SetContent(child.get<WidgetInstance>()->Value.ToSharedRef());
	}

	static inline void AttachToHorizontalBox(const flecs::entity child, const TSharedRef<SWidget> parent) {
		auto slot = StaticCastSharedRef<SHorizontalBox>(parent)->AddSlot();
		slot.AutoWidth();
		auto PATH_child = child.path();
		auto VALIGN = child.has<VAlign>();
		auto HALIGN = child.has<HAlign>();
		auto PADDING = child.has<Padding>();
		auto WIDGET = child.has<WidgetInstance>();
		AttachSlot(slot, child);
	}

	static inline void AttachToVerticalBox(const flecs::entity child, const TSharedRef<SWidget> parent) {
		auto slot = StaticCastSharedRef<SVerticalBox>(parent)->AddSlot();
		slot.AutoHeight();
		AttachSlot(slot, child);
	}

	static inline void AttachToCheckBox(const flecs::entity child, const TSharedRef<SWidget> parent) {
		auto checkBox = StaticCastSharedRef<SCheckBox>(parent);
		checkBox->SetContent(child.get<WidgetInstance>()->Value.ToSharedRef());
	}

	static inline void AttachToBorder(const flecs::entity child, const TSharedRef<SWidget> parent) {
		StaticCastSharedRef<SBorder>(parent)->SetContent(child.get<WidgetInstance>()->Value.ToSharedRef());
		//button.oncli
		////FSlateBrush normalBrush = FSlateRoundedBoxBrush(FLinearColor::White);
		//FSlateBrush normalBrush;
		//normalBrush.DrawAs = ESlateBrushDrawType::RoundedBox;

		//const FButtonStyle style = FButtonStyle()
		//	.SetNormal(normalBrush);


		//button->SetButtonStyle(&style);
	}

	int SortOrder(flecs::entity_t e1, const Order* o1, flecs::entity_t e2, const Order* o2) {
		(void)e1;
		(void)e2;
		return (o1->Value > o2->Value) - (o1->Value < o2->Value);
	}
}