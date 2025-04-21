// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "flecs.h"

namespace UIElements {
	struct ButtonFeature {
		static void RegisterComponents(flecs::world& world);
	};

	struct Button {};
}