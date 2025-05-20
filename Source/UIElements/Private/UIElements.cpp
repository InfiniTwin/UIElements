// Copyright Epic Games, Inc. All Rights Reserved.

#include "UIElements.h"
#include "UIFeature.h"
#include "TypographyFeature.h"
#include "WidgetFeature.h"
#include "ColorFeature.h"
#include "ButtonFeature.h"
#include "ToggleFeature.h"

#define LOCTEXT_NAMESPACE "FUIElementsModule"

void FUIElementsModule::StartupModule() {
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FUIElementsModule::ShutdownModule() {
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FUIElementsModule, UIElements)

namespace UIElements {
	void Register(flecs::world& world) {
		ColorFeature::RegisterOpaqueTypes(world);
		TypographyFeature::RegisterOpaqueTypes(world);

		UIFeature::RegisterComponents(world);
		WidgetFeature::RegisterComponents(world);
		ColorFeature::RegisterComponents(world);
		TypographyFeature::RegisterComponents(world);
		ButtonFeature::RegisterComponents(world);
		ToggleFeature::RegisterComponents(world);

		ColorFeature::CreateQueries(world);
		TypographyFeature::CreateQueries(world);

		UIFeature::CreateObservers(world);
		ColorFeature::CreateObservers(world);
		TypographyFeature::CreateObservers(world);

		WidgetFeature::CreateSystems(world);
		ColorFeature::CreateSystems(world);
		TypographyFeature::CreateSystems(world);
		ButtonFeature::CreateSystems(world);
	}
}