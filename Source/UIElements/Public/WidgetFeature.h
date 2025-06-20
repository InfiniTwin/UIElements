// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "flecs.h"
#include "UIFeature.h"

namespace UI {
	struct WidgetFeature {
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

	struct HAlign { int Value; };
	struct VAlign { int Value; };
	struct Padding { float Left, Top, Right, Bottom; };

	struct RoundedBoxBrush {};

	struct Attached {};
	struct StyleSynced {};

	struct Order { int Value; };

	template<typename TWidget>
	static inline void SetContent(const TSharedRef<SWidget>& parent, const flecs::entity& child) {
		StaticCastSharedRef<TWidget>(parent)->SetContent(child.get<WidgetInstance>()->Value.ToSharedRef());
	}

	template<typename SlotType>
	void AttachSlot(SlotType& slot, const flecs::entity child)
	{
		auto padding = child.get<Padding>();
		slot
			.VAlign(static_cast<EVerticalAlignment>(child.get<VAlign>()->Value))
			.HAlign(static_cast<EHorizontalAlignment>(child.get<HAlign>()->Value))
			.Padding(padding->Left, padding->Top, padding->Right, padding->Bottom)
			.AttachWidget(child.get<WidgetInstance>()->Value.ToSharedRef());
	}

	static inline void AttachToCompoundWidget(const TSharedRef<SWidget> child, const TSharedRef<SWidget> parent) {
		StaticCastSharedRef<CompoundWidgetInstance>(parent)->Slot().AttachWidget(child);
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

	static inline FMargin ToMargin(const Padding* padding) {
		return FMargin((padding->Left, padding->Top, padding->Right, padding->Bottom));
	}

	int SortOrder(flecs::entity_t e1, const Order* o1, flecs::entity_t e2, const Order* o2) {
		(void)e1;
		(void)e2;
		return (o1->Value > o2->Value) - (o1->Value < o2->Value);
	}
}