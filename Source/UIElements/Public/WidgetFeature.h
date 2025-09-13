// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "flecs.h"
#include "Widgets/Layout/SConstraintCanvas.h"
#include "Widgets/Input/SMenuAnchor.h"
#include "StyleFeature.h"

namespace UI {
	struct WidgetFeature {
		static void CreateComponents(flecs::world& world);
		static void CreateQueries(flecs::world& world);
		static void CreateObservers(flecs::world& world);
		static void CreateSystems(flecs::world& world);
	};

	struct SlateApplication {};
	struct Viewport {};

	struct UIScale { float Value; };

	struct UIOf {};

	struct Widget {};
	struct WidgetInstance { TSharedPtr<SWidget> Value; };

	struct ConstraintCanvas {};

	struct Collection {};

	struct Border {};

	struct Menu {};
	struct MenuPlacement { int Value; };
	struct MenuContent {};

	struct Box {};
	struct HBox {};
	struct VBox {};
	struct ScrollBox {};

	struct FillHeight { float Value; };
	struct FillWidth { float Value; };

	struct VAlign { int Value; };
	struct HAlign { int Value; };

	struct Orientation { int Value; };

	struct Padding { FMargin Value; };
	struct Position { FVector2D Value; };
	struct Size { FVector2D Value; };

	enum WidgetState {
		None,
		Opened,
		Closed
	};

	enum Visibility {
		Visible,
		Collapsed,
		Hidden,
		HitTestInvisible,
		SelfHitTestInvisible,
		All
	};

	struct Attached {};
	struct Order { int Value; };

	struct QueryUIOf { flecs::query<> Value; };

	static inline TPair<FAnchors, FVector2D> ToAnchorsAndAlignment(const flecs::entity& child) {
		EHorizontalAlignment hAlign = static_cast<EHorizontalAlignment>(child.try_get<HAlign>()->Value);
		EVerticalAlignment vAlign = static_cast<EVerticalAlignment>(child.try_get<VAlign>()->Value);

		FAnchors anchors;
		FVector2D alignment;

		switch (hAlign) {
		case HAlign_Left:
			anchors.Minimum.X = anchors.Maximum.X = 0.f;
			alignment.X = 0.f;
			break;
		case HAlign_Center:
			anchors.Minimum.X = anchors.Maximum.X = 0.5f;
			alignment.X = 0.5f;
			break;
		case HAlign_Right:
			anchors.Minimum.X = anchors.Maximum.X = 1.f;
			alignment.X = 1.f;
			break;
		case HAlign_Fill:
		default:
			anchors.Minimum.X = 0.f;
			anchors.Maximum.X = 1.f;
			alignment.X = 0.f;
			break;
		}

		switch (vAlign) {
		case VAlign_Top:
			anchors.Minimum.Y = anchors.Maximum.Y = 0.f;
			alignment.Y = 0.f;
			break;
		case VAlign_Center:
			anchors.Minimum.Y = anchors.Maximum.Y = 0.5f;
			alignment.Y = 0.5f;
			break;
		case VAlign_Bottom:
			anchors.Minimum.Y = anchors.Maximum.Y = 1.f;
			alignment.Y = 1.f;
			break;
		case VAlign_Fill:
		default:
			anchors.Minimum.Y = 0.f;
			anchors.Maximum.Y = 1.f;
			alignment.Y = 0.f;
			break;
		}

		return { anchors, alignment };
	}

	static inline void AddMenuWidget(const flecs::entity menu) {
		menu.set(WidgetInstance{ SNew(SMenuAnchor)
			.Placement(static_cast<EMenuPlacement>(menu.try_get<MenuPlacement>()->Value))
			.OnGetMenuContent(FOnGetContent::CreateLambda([menu]() -> TSharedRef<SWidget> {
				TSharedPtr<SWidget> content;
				menu.children([&content](flecs::entity child) {
					if (child.has<MenuContent>())
						content = child.try_get<WidgetInstance>()->Value;
				});
				return content.ToSharedRef(); }))
			.UseApplicationMenuStack(false)
			});
	}

	template<typename SlotType>
	void AttachSlot(SlotType& slot, const flecs::entity child) {
		slot
			.VAlign(static_cast<EVerticalAlignment>(child.try_get<VAlign>()->Value))
			.HAlign(static_cast<EHorizontalAlignment>(child.try_get<HAlign>()->Value))
			.Padding(child.try_get<Padding>()->Value)
			.AttachWidget(child.try_get<WidgetInstance>()->Value.ToSharedRef());
	}

	template<typename TWidget>
	static inline void SetContent(TSharedRef<SWidget>& parent, flecs::entity& child) {
		StaticCastSharedRef<TWidget>(parent)->SetContent(child.try_get<WidgetInstance>()->Value.ToSharedRef());
	}

	static inline void AttachToConstraintCanvas(TSharedRef<SWidget> parent, flecs::entity child) {
		auto [anchors, alignment] = ToAnchorsAndAlignment(child);
		StaticCastSharedRef<SConstraintCanvas>(parent)->AddSlot()
			.AutoSize(true)
			.Anchors(anchors)
			.Alignment(alignment)
			.Offset(child.try_get<Padding>()->Value)
			.AttachWidget(child.try_get<WidgetInstance>()->Value.ToSharedRef());
	}

	static inline void AttachToHorizontalBox(TSharedRef<SWidget> parent, flecs::entity child) {
		auto slot = StaticCastSharedRef<SHorizontalBox>(parent)->AddSlot();
		if (child.has<FillWidth>())
			slot.FillWidth(child.try_get<FillWidth>()->Value);
		else
			slot.AutoWidth();
		AttachSlot(slot, child);
	}

	static inline void AttachToVerticalBox(TSharedRef<SWidget> parent, flecs::entity child) {
		auto slot = StaticCastSharedRef<SVerticalBox>(parent)->AddSlot();
		if (child.has<FillHeight>())
			slot.FillHeight(child.try_get<FillHeight>()->Value);
		else
			slot.AutoHeight();
		AttachSlot(slot, child);
	}

	static inline void AttachToScrollBox(TSharedRef<SWidget> parent, flecs::entity child) {
		auto slot = StaticCastSharedRef<SScrollBox>(parent)->AddSlot();
		AttachSlot(slot, child);
	}

	inline int SortOrder(flecs::entity_t e1, const Order* o1, flecs::entity_t e2, const Order* o2) {
		(void)e1;
		(void)e2;
		return (o1->Value > o2->Value) - (o1->Value < o2->Value);
	}
}