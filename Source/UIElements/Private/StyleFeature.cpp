// Fill out your copyright notice in the Description page of Project Settings.


#include "StyleFeature.h"
#include "flecs.h"
#include "ECS.h"
#include "UIFeature.h"
#include "TypographyFeature.h"
#include "ButtonFeature.h"
#include "Styling/SlateStyleRegistry.h"
#include "Engine/UserInterfaceSettings.h"

namespace UI {
	void StyleFeature::RegisterComponents(flecs::world& world) {
		using namespace ECS;
		world.component<StyleSets>();
		world.component<WidgetStyle>().add(flecs::OnInstantiate, flecs::Inherit);

		world.component<Brush>().add(flecs::OnInstantiate, flecs::Inherit);
		world.component<BrushType>().member<int>(VALUE).add(flecs::OnInstantiate, flecs::Inherit);

		world.component<Width>().member<float>(VALUE).add(flecs::OnInstantiate, flecs::Inherit);
		world.component<FixedRadius>().add(flecs::OnInstantiate, flecs::Inherit);
		world.component<Radii>()
			.member<float>(MEMBER(Radii::TopLeft))
			.member<float>(MEMBER(Radii::TopRight))
			.member<float>(MEMBER(Radii::BottomRight))
			.member<float>(MEMBER(Radii::BottomLeft))
			.add(flecs::OnInstantiate, flecs::Inherit);
	}

	void StyleFeature::CreateQueries(flecs::world& world) {
		world.component<QueryButtonStylePrefab>();
		world.set(QueryButtonStylePrefab{
			world.query_builder<WidgetStyle>(COMPONENT(QueryButtonStylePrefab))
			.with<Button>()
			.without<Widget>()
			.with(flecs::Prefab)
			.cached().build() });
	};

	void StyleFeature::CreateObservers(flecs::world& world) {
		// Create widget style from prefab
			// For each child if child.name().contains "Normal"
				// Create brush from "Normal" child components and add to swidget style
					// If brush is round, create round brush from "Outline" child components and add to brush
		// Add widget style to StyleSets
		// Reference widget style by StyleName and add to ui element StyleName.OnSet (filter by widget type, Button, etc.)
		// On color update
			// Update widget style; 
				// Option 1: Try by reference so that ui elements auto update their styles
				// Option 2: loop all StyleName and reset

		world.observer<const UIScheme>("UpdateButtonStyles")
			.term_at(0).singleton()
			.event(flecs::OnSet)
			.each([&world](const UIScheme& scheme) {
			world.get<QueryButtonStylePrefab>()->Value
				.each([&world](flecs::entity prefab, WidgetStyle& style) {
				style.Value = AddButtonStyle(prefab);
				auto pp = prefab.path();

				TArray<flecs::entity> instances;
				ECS::GetInstances(world, prefab, instances);
				for (flecs::entity instance : instances)
				{
					auto ip = instance.path();
					StaticCastSharedPtr<SButton>(instance.get_mut<WidgetInstance>()->Value)->SetButtonStyle(&style.Value);
				}
					});
				});

		//world.observer<>("CreateWidgetStyle")
		//	.with<WidgetStyle>()
		//	.without<Widget>()
		//	.with(flecs::Prefab)
		//	.event(flecs::OnSet)
		//	.each([&world](flecs::entity prefab) {
		//	if (prefab.has<Button>()) {
		//		prefab.get_mut<WidgetStyle>()->Value = AddButtonStyle(prefab);

		//		world.each(world.pair(flecs::IsA, prefab), [](flecs::entity instance) {
		//			auto path = instance.path();
		//			});
		//	}
		//		});

		//world.observer<>("SetWidgetStyle")
		//	.with<WidgetStyle>()
		//	.with<Widget>()
		//	.with(flecs::Prefab)
		//	.event(flecs::OnSet)
		//	.each([&world](flecs::entity prefab) {
		//	if (prefab.has<Button>()) {
		//		//prefab.get_mut<WidgetStyle>()->Value = AddButtonStyle(prefab);

		//		world.each(world.pair(flecs::IsA, prefab), [](flecs::entity instance) {
		//			auto path = instance.path();
		//			});
		//	}
		//		});
	}

	void StyleFeature::CreateSystems(flecs::world& world) {
	}

	void StyleFeature::Initialize(flecs::world& world) {
		world.set<StyleSets>({ MakeShareable(new FSlateStyleSet(COMPONENT(Styles))) });

		if (FSlateStyleRegistry::FindSlateStyle(COMPONENT(Styles)))
			FSlateStyleRegistry::UnRegisterSlateStyle(*world.get<StyleSets>()->Value.Get());
		FSlateStyleRegistry::RegisterSlateStyle(*world.get<StyleSets>()->Value.Get());

		const FButtonStyle RoundButton = FButtonStyle()
			.SetNormal(FSlateRoundedBoxBrush(FLinearColor::White))
			.SetHovered(FSlateRoundedBoxBrush(FLinearColor::White))
			.SetPressed(FSlateRoundedBoxBrush(FLinearColor::White))
			.SetNormalPadding(FMargin(0))
			.SetPressedPadding(FMargin(0));

		auto ButtonStyle = FSlateWidgetStyle();

		world.get<StyleSets>()->Value.ToSharedRef()->Set(FName("DummyButton"), RoundButton);
	}
}