// Fill out your copyright notice in the Description page of Project Settings.


#include "UIFeature.h"
#include "flecs.h"
#include "ECS.h"
#include "OpaqueTypes.h"
#include "UIElements.h"
#include "Engine/UserInterfaceSettings.h"
#include "Logging/LogMacros.h"

namespace UIElements {
	void UIFeature::RegisterOpaqueTypes(flecs::world& world) {
		using namespace ECS;
		world.component<std::vector<float>>().opaque(ECS::VectorReflection<float>);
	}

	void UIFeature::RegisterComponents(flecs::world& world) {
		using namespace ECS;
		world.component<LoadMode>().add(flecs::Exclusive);

		world.component<Event>().add(flecs::Exclusive);

		world.component<Action>().add(flecs::OnInstantiate, flecs::Inherit);
		world.component<Path>().member<FString>(VALUE).add(flecs::OnInstantiate, flecs::Inherit);
		world.component<Parent>().member<FString>(VALUE).add(flecs::OnInstantiate, flecs::Inherit);

		world.component<UIScale>().member<float>(VALUE);
	}

	void UIFeature::CreateObservers(flecs::world& world) {
		world.observer<const UIScale>("UpdateApplicationUIScale")
			.term_at(0).singleton()
			.event(flecs::OnSet)
			.each([&world](const UIScale& scale) {
			GetMutableDefault<UUserInterfaceSettings>(UUserInterfaceSettings::StaticClass())
				->ApplicationScale = scale.Value; });

		world.observer<>("UndoActions")
			.with<Event>().second(flecs::Wildcard)
			.event(flecs::OnRemove)
			.each([&world](flecs::iter& it, size_t t) {
			auto event = it.pair(0);
			FString Name = UTF8_TO_TCHAR(event.second().name().c_str());
			UE_LOG(LogTemp, Warning, TEXT(">>> UNDO %s"), *Name);
			it.entity(t).children([&world, &event](flecs::entity child) {
				//if (child.has<Action>() && child.has(event)) {
				//	using namespace ECS;
				//	FString parent = child.has<Parent>() ? child.get<Parent>()->Value : "";
				//	FromJsonAsset(world, child.get<Path>()->Value, UIElements::Scope(), parent, *child.get<LoadMode>());
				//}
				});
				});

		world.observer<>("DoActions")
			.with<Event>().second(flecs::Wildcard)
			.event(flecs::OnSet)
			.each([&world](flecs::iter& it, size_t t) {
			auto event = it.pair(0);
			FString Name = UTF8_TO_TCHAR(event.second().name().c_str());
			UE_LOG(LogTemp, Warning, TEXT(">>> DO %s"), *Name);
			it.entity(t).children([&world, &event](flecs::entity child) {
				//if (child.has<Action>() && child.has(event)) {
				//	using namespace ECS;
				//	FString parent = child.has<Parent>() ? child.get<Parent>()->Value : "";
				//	FromJsonAsset(world, child.get<Path>()->Value, UIElements::Scope(), parent, *child.get<LoadMode>());
				//}
				});
				});

		world.observer<>("SetActionParent")
			.with<Action>()
			.with<Path>()
			.event(flecs::OnSet)
			.each([](flecs::entity entity) {
			if (entity.has<Parent>()) {
				FString parentPath = UTF8_TO_TCHAR(entity.parent().path().c_str());
				FString parent = entity.get<Parent>()->Value;
				if (!parent.IsEmpty())
					parentPath += TEXT(".") + parent;
				using namespace ECS;
				entity.set<Parent>({ NormalizedPath(parentPath) });
			}
				});
	}
}