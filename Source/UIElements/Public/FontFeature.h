// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "flecs.h"

namespace UIElements {
	struct FontFeature {
		static void RegisterComponents(flecs::world& world);
		static void RegisterSystems(flecs::world& world);
		static void Initialize(flecs::world& world);

		static void UpdateTextStyles(flecs::world& world);
		static void AwaitDelay(flecs::entity entity, float seconds, std::function<void()> onCompleted);
	};

	struct TextFont { FString Value; };

	struct TextStyles { TSharedPtr<FSlateStyleSet> Value; };

	struct TextTypes {
		FSlateFontInfo  DisplayLarge;
		FSlateFontInfo  DisplayMedium;
		FSlateFontInfo  DisplaySmall;
		FSlateFontInfo  HeadlineLarge;
		FSlateFontInfo  HeadlineMedium;
		FSlateFontInfo  HeadlineSmall;
		FSlateFontInfo  TitleLarge;
		FSlateFontInfo  TitleMedium;
		FSlateFontInfo  TitleSmall;
		FSlateFontInfo  BodyLarge;
		FSlateFontInfo  BodyMedium;
		FSlateFontInfo  BodySmall;
		FSlateFontInfo  LabelLarge;
		FSlateFontInfo  LabelMedium;
		FSlateFontInfo  LabelSmall;
	};
}