// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"
#include "flecs.h"

class FUIElementsModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

namespace UI {
	UIELEMENTS_API FString& Scope();

	UIELEMENTS_API void Register(flecs::world& world, const FString scope);
}
