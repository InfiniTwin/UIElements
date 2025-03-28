// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "flecs.h"

namespace UIElements {
	struct UIELEMENTS_API Module {
	public:
		Module(flecs::world& world);
	};
}