// Fill out your copyright notice in the Description page of Project Settings.


#include "Module.h"
#include "Config/ConfigComponents.h"
#include "Config/ConfigSystems.h"

namespace UIElements {
	Module::Module(flecs::world& world) {
		world.module<Module>();

		ConfigComponents::Register(world);
		ConfigSystems::Register(world);
	}
}