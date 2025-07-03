// Copyright Epic Games, Inc. All Rights Reserved.

#include "UIElements.h"
#include "UIFeature.h"
#include "TypographyFeature.h"
#include "WidgetFeature.h"
#include "ColorFeature.h"
#include "StyleFeature.h"
#include "ButtonFeature.h"

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

		UIFeature::RegisterComponents(world);
		WidgetFeature::RegisterComponents(world);
		ColorFeature::RegisterComponents(world);
		TypographyFeature::RegisterComponents(world);
		StyleFeature::RegisterComponents(world);
		ButtonFeature::RegisterComponents(world);

		ColorFeature::CreateQueries(world);
		TypographyFeature::CreateQueries(world);
		StyleFeature::CreateQueries(world);
		ButtonFeature::CreateQueries(world);

		UIFeature::CreateObservers(world);
		WidgetFeature::CreateObservers(world);
		ColorFeature::CreateObservers(world);
		StyleFeature::CreateObservers(world);
		TypographyFeature::CreateObservers(world);
		ButtonFeature::CreateObservers(world);

		WidgetFeature::CreateSystems(world);
	}
}