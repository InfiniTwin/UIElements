// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "flecs.h"
#include "ECS.h"
#include "Widgets/Layout/SConstraintCanvas.h"
#include "Widgets/Input/SMenuAnchor.h"

namespace UI {
	struct WidgetFeature {
		static void RegisterComponents(flecs::world& world);
		static void CreateObservers(flecs::world& world);
		static void CreateSystems(flecs::world& world);
	};

	struct Viewport {};

	struct Widget {};
	struct WidgetInstance { TSharedPtr<SWidget> Value; };

	struct ConstraintCanvas {};

	struct CompoundWidgetInstance : public SCompoundWidget {
	public:
		SLATE_BEGIN_ARGS(CompoundWidgetInstance) {}
		SLATE_END_ARGS()

		void Construct(const FArguments& InArgs) {}

		FCompoundWidgetOneChildSlot& Slot() { return ChildSlot; }
	};
	struct CompoundWidget {};

	struct Menu {};

	struct Attached {};
	struct Order { int Value; };

	struct Box {};
	struct HBox {};
	struct VBox {};

	struct HAlign { int Value; };
	struct VAlign { int Value; };
	struct Padding { float Left, Top, Right, Bottom; };

	struct Open {};

	struct MenuPlacement { int Value; };

	static inline FMargin ToMargin(const Padding* padding) {
		return FMargin((padding->Left, padding->Top, padding->Right, padding->Bottom));
	}

	static inline TPair<FAnchors, FVector2D> ToAnchorsAndAlignment(const flecs::entity& child)
	{
		EVerticalAlignment vAlign = static_cast<EVerticalAlignment>(child.get<VAlign>()->Value);
		EHorizontalAlignment hAlign = static_cast<EHorizontalAlignment>(child.get<HAlign>()->Value);

		FAnchors anchors;
		FVector2D alignment;

		switch (hAlign)
		{
		case HAlign_Left:   anchors.Minimum.X = anchors.Maximum.X = 0.f; alignment.X = 0.f; break;
		case HAlign_Center: anchors.Minimum.X = anchors.Maximum.X = 0.5f; alignment.X = 0.5f; break;
		case HAlign_Right:  anchors.Minimum.X = anchors.Maximum.X = 1.f; alignment.X = 1.f; break;
		default:            anchors.Minimum.X = anchors.Maximum.X = 0.f; alignment.X = 0.f; break;
		}

		switch (vAlign)
		{
		case VAlign_Top:    anchors.Minimum.Y = anchors.Maximum.Y = 0.f; alignment.Y = 0.f; break;
		case VAlign_Center: anchors.Minimum.Y = anchors.Maximum.Y = 0.5f; alignment.Y = 0.5f; break;
		case VAlign_Bottom: anchors.Minimum.Y = anchors.Maximum.Y = 1.f; alignment.Y = 1.f; break;
		default:            anchors.Minimum.Y = anchors.Maximum.Y = 0.f; alignment.Y = 0.f; break;
		}

		return { anchors, alignment };
	}

	static inline void AddMenuWidget(const flecs::entity entity) {
		entity.set(WidgetInstance{ SNew(SMenuAnchor)
			.Placement(static_cast<EMenuPlacement>(entity.get<MenuPlacement>()->Value))
			.OnGetMenuContent(FOnGetContent::CreateLambda([entity]() {
				return ECS::FirstChild(entity).get<WidgetInstance>()->Value.ToSharedRef();
				}))
			[entity.parent().get<WidgetInstance>()->Value.ToSharedRef()]
			});

		//// Cache references first
		//TSharedRef<SMenuAnchor> MenuAnchor = SNew(SMenuAnchor)
		//	.Placement(static_cast<EMenuPlacement>(entity.get<MenuPlacement>()->Value))
		//	.OnGetMenuContent(FOnGetContent::CreateLambda([&entity]() {
		//	return ECS::FirstChild(entity).get<WidgetInstance>()->Value.ToSharedRef();
		//		}));

		//TSharedRef<SWidget> ToggleWidget = entity.parent().get<WidgetInstance>()->Value.ToSharedRef();

		//// Cast ToggleWidget to SCheckBox (make sure it's valid!)
		//TSharedPtr<SCheckBox> ToggleCheckBox = StaticCastSharedRef<SCheckBox>(ToggleWidget);

		//MenuAnchor->SetOnMenuOpenChanged(FOnIsOpenChanged::CreateLambda([ToggleCheckBox](bool bIsOpen) {
		//	if (ToggleCheckBox.IsValid()) {
		//		ToggleCheckBox->SetIsChecked(bIsOpen ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
		//	}
		//	}));

		//ToggleCheckBox->SetOnCheckStateChanged(FOnCheckStateChanged::CreateLambda([MenuAnchor](ECheckBoxState NewState) {
		//	const bool bShouldOpen = (NewState == ECheckBoxState::Checked);
		//	if (MenuAnchor.IsValid()) {
		//		MenuAnchor->SetIsOpen(bShouldOpen, false);
		//	}
		//	}));

		//// Now set the WidgetInstance on the entity
		//entity.set(WidgetInstance{ MenuAnchor });

		//TSharedPtr<SMenuAnchor> MenuAnchor;
		//TSharedPtr<SCheckBox> ToggleCheckBox;

		//bool bToggleMenuOpen = false; // This acts like A_CHECK

		//SAssignNew(MenuAnchor, SMenuAnchor)
		//	.Placement(MenuPlacement_ComboBox)
		//	.OnGetMenuContent(FOnGetContent::CreateLambda([]()
		//		{
		//			return SNew(SVerticalBox)
		//				+ SVerticalBox::Slot().AutoHeight()
		//				[
		//					SNew(STextBlock).Text(FText::FromString("Menu Item 1"))
		//				]
		//				+ SVerticalBox::Slot().AutoHeight()
		//				[
		//					SNew(STextBlock).Text(FText::FromString("Menu Item 2"))
		//				];
		//		}))
		//	.OnMenuOpenChanged_Lambda([&](bool bIsOpen)
		//		{
		//			bToggleMenuOpen = bIsOpen;

		//			// Optionally update checkbox manually if needed
		//			if (ToggleCheckBox.IsValid())
		//			{
		//				ToggleCheckBox->SetIsChecked(bIsOpen ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
		//			}
		//		})
		//	[
		//		SAssignNew(ToggleCheckBox, SCheckBox)
		//			.IsChecked_Lambda([&]()
		//				{
		//					return bToggleMenuOpen ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
		//				})
		//			.OnCheckStateChanged_Lambda([&](ECheckBoxState NewState)
		//				{
		//					const bool bShouldOpen = (NewState == ECheckBoxState::Checked);
		//					bToggleMenuOpen = bShouldOpen;

		//					if (MenuAnchor.IsValid())
		//					{
		//						MenuAnchor->SetIsOpen(bShouldOpen, false);
		//					}
		//				})
		//			[
		//				SNew(STextBlock).Text(FText::FromString("Perspective"))
		//			]
		//	];


		//// Define the shared pointer type for language options
		//TArray<TSharedPtr<FString>> LanguageOptions;
		//TSharedPtr<FString> SelectedLanguage;

		//// Populate the list of languages (for example)
		//LanguageOptions.Add(MakeShared<FString>(TEXT("English")));
		//LanguageOptions.Add(MakeShared<FString>(TEXT("Deutsch")));
		//LanguageOptions.Add(MakeShared<FString>(TEXT("Shqip")));

		//// ComboBox widget
		//SNew(SComboBox<TSharedPtr<FString>>)
		//	.OptionsSource(&LanguageOptions)
		//	.OnGenerateWidget_Lambda([](TSharedPtr<FString> InItem) {
		//	return SNew(STextBlock).Text(FText::FromString(*InItem));
		//		})
		//	.OnSelectionChanged_Lambda([&](TSharedPtr<FString> NewSelection, ESelectInfo::Type) {
		//	SelectedLanguage = NewSelection;
		//	// Your logic to change language
		//	UE_LOG(LogTemp, Log, TEXT("Language changed to: %s"), **NewSelection);
		//		})
		//	.InitiallySelectedItem(SelectedLanguage)
		//	[
		//		SNew(STextBlock)
		//			.Text_Lambda([&]() {
		//			return FText::FromString(SelectedLanguage.IsValid() ? *SelectedLanguage : TEXT("Select Language"));
		//				})
		//	];
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

	template<typename TWidget>
	static inline void SetContent(const TSharedRef<SWidget>& parent, const flecs::entity& child) {
		StaticCastSharedRef<TWidget>(parent)->SetContent(child.get<WidgetInstance>()->Value.ToSharedRef());
	}

	static inline void AttachToCompoundWidget(const TSharedRef<SWidget> child, const TSharedRef<SWidget> parent) {
		StaticCastSharedRef<CompoundWidgetInstance>(parent)->Slot().AttachWidget(child);
	}

	static inline void AttachToConstraintCanvas(const TSharedRef<SWidget> parent, const flecs::entity child) {
		auto [anchors, alignment] = ToAnchorsAndAlignment(child);
		StaticCastSharedRef<SConstraintCanvas>(parent)->AddSlot()
			.AutoSize(true)
			.Anchors(anchors)
			.Alignment(alignment)
			.Offset(ToMargin(child.get<Padding>()))
			.AttachWidget(child.get<WidgetInstance>()->Value.ToSharedRef());
	}

	static inline void AttachToBox(const TSharedRef<SWidget> parent, const flecs::entity child) {
		auto slot = StaticCastSharedRef<SHorizontalBox>(parent)->AddSlot();
		slot.AutoWidth();
		AttachSlot(slot, child);
	}

	static inline void AttachToHorizontalBox(const TSharedRef<SWidget> parent, const flecs::entity child) {
		auto slot = StaticCastSharedRef<SHorizontalBox>(parent)->AddSlot();
		slot.AutoWidth();
		AttachSlot(slot, child);
	}

	static inline void AttachToVerticalBox(const TSharedRef<SWidget> parent, const flecs::entity child) {
		auto slot = StaticCastSharedRef<SVerticalBox>(parent)->AddSlot();
		slot.AutoHeight();
		AttachSlot(slot, child);
	}

	int SortOrder(flecs::entity_t e1, const Order* o1, flecs::entity_t e2, const Order* o2) {
		(void)e1;
		(void)e2;
		return (o1->Value > o2->Value) - (o1->Value < o2->Value);
	}
}