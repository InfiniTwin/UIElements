// Fill out your copyright notice in the Description page of Project Settings.


#include "UIFeature.h"
#include "flecs.h"
#include "ECS.h"

namespace UIElements {
	void UIFeature::RegisterComponents(flecs::world& world) {
		world.component<UIScale>().member<double>(MEMBER(UIScale::Value));
	}

	void UIFeature::Initialize(flecs::world& world) {
		ECS::FromJsonAsset(world, DefaultUIStyle);
	}
}
