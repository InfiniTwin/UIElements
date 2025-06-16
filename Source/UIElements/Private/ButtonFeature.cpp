// Fill out your copyright notice in the Description page of Project Settings.

#include "ButtonFeature.h"
#include "WidgetFeature.h"

namespace UI {
	void ButtonFeature::RegisterComponents(flecs::world& world) {
		world.component<Border>().add(flecs::OnInstantiate, flecs::Inherit);
		world.component<Button>().add(flecs::OnInstantiate, flecs::Inherit);
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