// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "flecs.h"

namespace UI {
	struct UICORE_API UICoreModule {
	public:
		UICoreModule(flecs::world& world);
	};
}