// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "flecs.h"

namespace UIElements {
	struct ToggleFeature {
		static void RegisterComponents(flecs::world& world);
	};

	struct Toggle {};
}