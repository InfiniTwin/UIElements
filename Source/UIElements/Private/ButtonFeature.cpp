// Fill out your copyright notice in the Description page of Project Settings.

#include "ButtonFeature.h"
#include "WidgetFeature.h"

namespace UI {
	void ButtonFeature::RegisterOpaqueTypes(flecs::world& world) {
		// ESlateBrushDrawType <=> int
		world.component<BrushType>()
			.opaque(flecs::I32)
			.serialize([](const flecs::serializer* s, const BrushType* data)
				{
					int value = static_cast<int>(data->Value);
					return s->value(flecs::I32, &value);
				})
			.assign_int([](BrushType* data, int64_t value)
				{
					if (value >= static_cast<int64_t>(ESlateBrushDrawType::NoDrawType) &&
						value <= static_cast<int64_t>(ESlateBrushDrawType::RoundedBox)) {
						data->Value = static_cast<ESlateBrushDrawType::Type>(value);
					}
					else {
						data->Value = ESlateBrushDrawType::NoDrawType;
					}
				});
	};

	void ButtonFeature::RegisterComponents(flecs::world& world) {
		world.component<Button>();
		world.component<Border>();
	};

	void ButtonFeature::CreateSystems(flecs::world& world) {
		world.system("AddButtonWidget")
			.with<Button>()
			.without<WidgetInstance>()
			.each([](flecs::entity e) { /*e.set(Widget{ SNew(SButton) });*/ });
			//.each([](flecs::entity e) {
			//TSharedPtr<SButton> button = SNew(SButton);
			////button->SetHAlign(HAlign_Fill);
			////button->SetVAlign(VAlign_Fill);
			//
			//static const FButtonStyle RoundButton = FButtonStyle()
			//	.SetNormal(FSlateRoundedBoxBrush(FLinearColor::Red, 4))
			//	.SetHovered(FSlateRoundedBoxBrush(FLinearColor::Blue, 4))
			//	.SetPressed(FSlateRoundedBoxBrush(FLinearColor::Green, 4))
			//	.SetNormalPadding(FMargin(0))
			//	.SetPressedPadding(FMargin(0));
			//button->SetButtonStyle(&RoundButton);

			//e.set(Widget{ button });

			////StaticCastSharedPtr<SButton>(e.get_mut<Widget>()->Value)->Set
			//	});
	}
}