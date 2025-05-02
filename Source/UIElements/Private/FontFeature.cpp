// Fill out your copyright notice in the Description page of Project Settings.


#include "FontFeature.h"
#include "ECS.h"
#include "UIFeature.h"
#include "Styling/SlateStyleRegistry.h"
#include "Engine/UserInterfaceSettings.h"

namespace UIElements {
	void FontFeature::RegisterComponents(flecs::world& world) {
		world.component<TextFont>().member<FString>(VALUE);
		world.component<TextStyles>();
	}

	void FontFeature::RegisterSystems(flecs::world& world) {}

	void FontFeature::Initialize(flecs::world& world) {
		world.set<TextStyles>({ MakeShareable(new FSlateStyleSet(COMPONENT(TextStyles))) });
		if (FSlateStyleRegistry::FindSlateStyle(COMPONENT(TextStyles))) {
			FSlateStyleRegistry::UnRegisterSlateStyle(*world.get<TextStyles>()->Value.Get());
		}
		FSlateStyleRegistry::RegisterSlateStyle(*world.get<TextStyles>()->Value.Get());

		UpdateTextStyles(world);

		//TSharedPtr<SWindow> SlateWindow =
		//	SNew(SWindow)
		//	.Title(FText::FromString(TEXT("Slate Test")))
		//	.ClientSize(FVector2D(400, 200))
		//	.IsTopmostWindow(true)
		//	.UseOSWindowBorder(true)
		//	.CreateTitleBar(true)
		//	.SaneWindowPlacement(true)
		//	[
		//		SNew(STextBlock)
		//			//.Font(world.get<TextStyles>()->Value->GetFontStyle(MEMBER(TextTypes::DisplayLarge)))
		//			//.TextStyle(&world.get<TextStyles>()->Value.Get()->GetWidgetStyle<FTextBlockStyle>(MEMBER(TextTypes::DisplayLarge)))
		//			.Text(FText::FromString(TEXT("Slate Test")))
		//			.Font_Lambda([&world]() -> FSlateFontInfo
		//				{
		//					return world.get<TextStyles>()->Value->GetFontStyle(MEMBER(TextTypes::DisplayLarge));
		//				})
		//			.Justification(ETextJustify::Center)
		//	];

		//FSlateApplication::Get().AddWindow(SlateWindow.ToSharedRef());

		//world.system<Delay>()
		//	.each([](flecs::entity e, Delay& delay) {
		//	delay.RemainingTime -= e.world().delta_time();
		//	if (delay.RemainingTime <= 0.f) {
		//		if (delay.Callback) {
		//			delay.Callback();
		//		}
		//		e.remove<Delay>();
		//	}
		//		});

		//flecs::entity myEntity = world.entity();

		//AwaitDelay(myEntity, 5.f, [&world]() {
		//	UE_LOG(LogTemp, Warning, TEXT(">>> 5 seconds have passed!"));
		//	world.set<TextFont>({ "Roboto" });
		//	UpdateTextStyles(world);
		//	});

		//world.set<IconFont>({ IconFont() });
		//world.set<IconFontFill>({ IconFontFill() });
	}

	void FontFeature::UpdateTextStyles(flecs::world& world) {
		auto uiScale = world.get<UIScale>()->Value;
		GetMutableDefault<UUserInterfaceSettings>(UUserInterfaceSettings::StaticClass())->ApplicationScale = world.get<UIScale>()->Value;

		const FString FontName = world.get<TextFont>()->Value;
		const FString font400 = FPaths::ProjectContentDir() / TEXT("Slate/Fonts/") + FontName + TEXT("400.ttf");
		const FString font500 = FPaths::ProjectContentDir() / TEXT("Slate/Fonts/") + FontName + TEXT("500.ttf");

		TextTypes textTypes = {
			FSlateFontInfo(font400, 32), // DisplayLarge
			FSlateFontInfo(font400, 28), // DisplayMedium
			FSlateFontInfo(font400, 24), // DisplaySmall
			FSlateFontInfo(font400, 22), // HeadlineLarge
			FSlateFontInfo(font400, 20), // HeadlineMedium
			FSlateFontInfo(font400, 18), // HeadlineSmall
			FSlateFontInfo(font400, 16), // TitleLarge
			FSlateFontInfo(font500, 14), // TitleMedium
			FSlateFontInfo(font500, 12), // TitleSmall
			FSlateFontInfo(font400, 12), // BodyLarge
			FSlateFontInfo(font400, 11), // BodyMedium
			FSlateFontInfo(font400, 10), // BodySmall
			FSlateFontInfo(font500, 14), // LabelLarge
			FSlateFontInfo(font500, 12), // LabelMedium
			FSlateFontInfo(font500, 10)  // LabelSmall
		};

		world.get<TextStyles>()->Value.ToSharedRef()->Set(FName(UTF8_TO_TCHAR(MEMBER(TextTypes::DisplayLarge))), textTypes.DisplayLarge);
		world.get<TextStyles>()->Value.ToSharedRef()->Set(FName(UTF8_TO_TCHAR(MEMBER(TextTypes::DisplayMedium))), textTypes.DisplayMedium);
		world.get<TextStyles>()->Value.ToSharedRef()->Set(FName(UTF8_TO_TCHAR(MEMBER(TextTypes::DisplaySmall))), textTypes.DisplaySmall);
		world.get<TextStyles>()->Value.ToSharedRef()->Set(FName(UTF8_TO_TCHAR(MEMBER(TextTypes::HeadlineLarge))), textTypes.HeadlineLarge);
		world.get<TextStyles>()->Value.ToSharedRef()->Set(FName(UTF8_TO_TCHAR(MEMBER(TextTypes::HeadlineMedium))), textTypes.HeadlineMedium);
		world.get<TextStyles>()->Value.ToSharedRef()->Set(FName(UTF8_TO_TCHAR(MEMBER(TextTypes::HeadlineSmall))), textTypes.HeadlineSmall);
		world.get<TextStyles>()->Value.ToSharedRef()->Set(FName(UTF8_TO_TCHAR(MEMBER(TextTypes::TitleLarge))), textTypes.TitleLarge);
		world.get<TextStyles>()->Value.ToSharedRef()->Set(FName(UTF8_TO_TCHAR(MEMBER(TextTypes::TitleMedium))), textTypes.TitleMedium);
		world.get<TextStyles>()->Value.ToSharedRef()->Set(FName(UTF8_TO_TCHAR(MEMBER(TextTypes::TitleSmall))), textTypes.TitleSmall);
		world.get<TextStyles>()->Value.ToSharedRef()->Set(FName(UTF8_TO_TCHAR(MEMBER(TextTypes::BodyLarge))), textTypes.BodyLarge);
		world.get<TextStyles>()->Value.ToSharedRef()->Set(FName(UTF8_TO_TCHAR(MEMBER(TextTypes::BodyMedium))), textTypes.BodyMedium);
		world.get<TextStyles>()->Value.ToSharedRef()->Set(FName(UTF8_TO_TCHAR(MEMBER(TextTypes::BodySmall))), textTypes.BodySmall);
		world.get<TextStyles>()->Value.ToSharedRef()->Set(FName(UTF8_TO_TCHAR(MEMBER(TextTypes::LabelLarge))), textTypes.LabelLarge);
		world.get<TextStyles>()->Value.ToSharedRef()->Set(FName(UTF8_TO_TCHAR(MEMBER(TextTypes::LabelMedium))), textTypes.LabelMedium);
		world.get<TextStyles>()->Value.ToSharedRef()->Set(FName(UTF8_TO_TCHAR(MEMBER(TextTypes::LabelSmall))), textTypes.LabelSmall);

		//FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
		//FSlateApplication::Get().InvalidateAllWidgets(true);

		//FSlateStyleRegistry::UnRegisterSlateStyle(*world.get<TextStyles>()->Value.Get());
		//FSlateStyleRegistry::RegisterSlateStyle(*world.get<TextStyles>()->Value.Get());
	}

	void FontFeature::AwaitDelay(flecs::entity entity, float seconds, std::function<void()> onCompleted) 	{
		entity.set<Delay>({ seconds, std::move(onCompleted) });
	}
}