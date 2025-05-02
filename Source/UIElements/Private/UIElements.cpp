// Copyright Epic Games, Inc. All Rights Reserved.

#include "UIElements.h"
#include "UIFeature.h"
#include "TextFeature.h"
#include "WidgetFeature.h"
#include "FontFeature.h"
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

void UIElements::Register(flecs::world& world) {
	TextFeature::RegisterOpaqueTypes(world);
	ColorFeature::RegisterOpaqueTypes(world);

	UIFeature::RegisterComponents(world);
	TextFeature::RegisterComponents(world);
	WidgetFeature::RegisterComponents(world);
	ColorFeature::RegisterComponents(world);
	FontFeature::RegisterComponents(world);
	ButtonFeature::RegisterComponents(world);
	ToggleFeature::RegisterComponents(world);

	TextFeature::CreateQueries(world);

	TextFeature::RegisterObservers(world);
	WidgetFeature::RegisterObservers(world);

	ColorFeature::RegisterSystems(world);
	FontFeature::RegisterSystems(world);

	UIFeature::Initialize(world);
	ColorFeature::Initialize(world);
	FontFeature::Initialize(world);
	WidgetFeature::Initialize(world);
}