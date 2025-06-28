// Fill out your copyright notice in the Description page of Project Settings.

#include "StyleFeature.h"
#include "flecs.h"
#include "WidgetFeature.h"

namespace UI {
	void StyleFeature::RegisterComponents(flecs::world& world) {
		using namespace ECS;
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
		world.component<QueryBrushPrefab>();
		world.set(QueryBrushPrefab{
			world.query_builder<Brush>(COMPONENT(QueryBrushPrefab))
			.with(flecs::Prefab)
			.cached().build() });
	};

	void StyleFeature::CreateObservers(flecs::world& world) {
		world.observer<>("SetWidgetBrushOnBrushAdded")
			.with<Brush>()
			.event(flecs::OnAdd)
			.each([&world](flecs::entity brush) {
			flecs::entity parent = brush.parent();
			if (parent.has<WidgetInstance>() && parent.has<Border>())
				StaticCastSharedRef<SBorder>(parent.get_mut<WidgetInstance>()->Value.ToSharedRef())
				->SetBorderImage(&brush.get<Brush>()->Value);
				});

		world.observer<>("SetWidgetBrushOnWidgetInstanceAdded")
			.with<WidgetInstance>()
			.event(flecs::OnSet)
			.each([&world](flecs::entity widget) {
			widget.children([&widget](flecs::entity child) {
				if (child.has<Brush>()) {
					if (widget.has<Border>())
						StaticCastSharedRef<SBorder>(widget.get_mut<WidgetInstance>()->Value.ToSharedRef())
						->SetBorderImage(&child.get<Brush>()->Value);
				}}); });
	}
}