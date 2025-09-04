// Copyright Epic Games, Inc. All Rights Reserved.

#include "UIElements.h"
#include "TypographyFeature.h"
#include "WidgetFeature.h"
#include "ColorFeature.h"
#include "StyleFeature.h"
#include "ButtonFeature.h"
#include "WindowFeature.h"

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

namespace UI {
	FString& Scope() {
		static FString scope = TEXT("");
		return scope;
	}

	void Register(flecs::world& world) {
		ColorFeature::RegisterOpaqueTypes(world);

		WidgetFeature::CreateComponents(world);
		ColorFeature::CreateComponents(world);
		TypographyFeature::CreateComponents(world);
		StyleFeature::CreateComponents(world);
		ButtonFeature::CreateComponents(world);
		WindowFeature::CreateComponents(world);

		WidgetFeature::CreateQueries(world);
		ColorFeature::CreateQueries(world);
		TypographyFeature::CreateQueries(world);
		StyleFeature::CreateQueries(world);
		ButtonFeature::CreateQueries(world);
		WindowFeature::CreateQueries(world);

		WidgetFeature::CreateObservers(world);
		ColorFeature::CreateObservers(world);
		StyleFeature::CreateObservers(world);
		TypographyFeature::CreateObservers(world);
		ButtonFeature::CreateObservers(world);
		WindowFeature::CreateObservers(world);

		WidgetFeature::CreateSystems(world);

		WindowFeature::Initialize(world);
	}
}