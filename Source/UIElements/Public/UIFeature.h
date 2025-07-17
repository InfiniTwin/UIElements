// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <flecs.h>

namespace UI {
	struct UIScale { float Value; };

	struct UIFeature {
		static void RegisterComponents(flecs::world& world);
		static void CreateObservers(flecs::world& world);
	};

	enum WidgetState {
		None,
		Opened,
		Closed
	};

	enum ButtonState {
		Hovered,
		Unhovered,
		Pressed,
		Released,
		Clicked
	};

	enum CheckBoxState {
		Unchecked,
		Checked,
		Undetermined
	};
}