// Fill out your copyright notice in the Description page of Project Settings.

#include "Theme/ThemeComponents.h"
#include "CoreMinimal.h"
#include "flecs.h"
#include "World.h"

namespace UI {
	void ThemeComponents::Register(flecs::world& world) {
		world.component<FString>()
			.opaque(flecs::String)
			.serialize([](const flecs::serializer* s, const FString* data) {
			const char* str = TCHAR_TO_UTF8(**data);
			return s->value(flecs::String, &str);
				})
			.assign_string([](FString* data, const char* value) {
			*data = UTF8_TO_TCHAR(value);
				});

		world.component<UIScale>().member<float>("Value");
		world.component<TextFont>().member<FString>("Value");

		World::DeserializeSingletons(world, "UIConfig");
	}
}