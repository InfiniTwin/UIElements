// Fill out your copyright notice in the Description page of Project Settings.


#include "Module.h"
#include "UIFeature.h"
#include "FontFeature.h"
#include "ColorFeature.h"

namespace UIElements {
	Module::Module(flecs::world& world) {
		world.module<Module>();

		UIFeature::RegisterOpaqueTypes(world);
		ColorFeature::RegisterOpaqueTypes(world);

		UIFeature::RegisterComponents(world);
		FontFeature::RegisterComponents(world);
		ColorFeature::RegisterComponents(world);

		FontFeature::RegisterSystems(world);
		ColorFeature::RegisterSystems(world);

		ColorFeature::Initialize(world);
	}
}