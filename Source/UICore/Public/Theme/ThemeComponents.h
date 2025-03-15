// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "flecs.h"

namespace UI {

	struct ThemeComponents
	{
	public:
		static void Register(flecs::world& world);
	};

	struct UIScale {
		float Value = 1;
	};
}