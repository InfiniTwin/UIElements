// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "flecs.h"

namespace UIElements {

	struct ConfigSystems {
		static void Register(flecs::world& world);
		static void Initialize(flecs::world& world);
	};
}