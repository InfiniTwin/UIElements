// Fill out your copyright notice in the Description page of Project Settings.


#include "UIFeature.h"
#include "flecs.h"
#include "ECS.h"
#include "UIElements.h"
#include "Engine/UserInterfaceSettings.h"

namespace UIElements {
	void UIFeature::RegisterComponents(flecs::world& world) {
		world.component<UIScale>().member<double>(VALUE);
	}

	void UIFeature::CreateObservers(flecs::world& world) {
		world.observer<const UIScale>("UpdateApplicationUIScale")
			.term_at(0).singleton()
			.event(flecs::OnSet)
			.each([&world](const UIScale& scale) {
			GetMutableDefault<UUserInterfaceSettings>(UUserInterfaceSettings::StaticClass())
				->ApplicationScale = scale.Value; });
	}

	void UIFeature::Initialize(flecs::world& world) {
		ECS::FromJsonAsset(world, "UIColor", UIElements::Scope());
		ECS::FromJsonAsset(world, "UITypography", UIElements::Scope());
	}
}
