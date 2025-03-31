// Fill out your copyright notice in the Description page of Project Settings.


#include "UIFeature.h"
#include "flecs.h"
#include "Component.h"

namespace UIElements {
	void UIFeature::RegisterOpaqueTypes(flecs::world& world) {
		// FString <=> std::string
		world.component<FString>()
			.opaque(flecs::String)
			.serialize([](const flecs::serializer* s, const FString* data)
				{
					const char* str = TCHAR_TO_UTF8(**data);
					return s->value(flecs::String, &str);
				})
			.assign_string([](FString* data, const char* value)
				{
					*data = UTF8_TO_TCHAR(value);
				});
	}

	void UIFeature::RegisterComponents(flecs::world& world) {
		world.component<UIScale>().member<double>(NAMEOF_MEMBER(UIScale::Value));
	}
}
