// Fill out your copyright notice in the Description page of Project Settings.

#include "ButtonFeature.h"
#include "WidgetFeature.h"
#include "ActionFeature.h"

namespace UI {
	void ButtonFeature::RegisterComponents(flecs::world& world) {
		world.component<Button>().add(flecs::OnInstantiate, flecs::Inherit);
		world.component<ButtonStyle>().add(flecs::OnInstantiate, flecs::Inherit);

		world.component<CheckBox>().add(flecs::OnInstantiate, flecs::Inherit);
		world.component<CheckBoxStyle>().add(flecs::OnInstantiate, flecs::Inherit);

		world.component<Toggle>().add(flecs::OnInstantiate, flecs::Inherit);
		world.component<Radio>().add(flecs::OnInstantiate, flecs::Inherit);
	};

	void ButtonFeature::CreateQueries(flecs::world& world) {
		world.component<QueryButtonStylePrefab>();
		world.set(QueryButtonStylePrefab{
			world.query_builder<ButtonStyle>(COMPONENT(QueryButtonStylePrefab))
			.with<Button>()
			.without<Widget>()
			.with(flecs::Prefab)
			.cached().build() });

		world.component<QueryCheckBoxStylePrefab>();
		world.set(QueryCheckBoxStylePrefab{
			world.query_builder<CheckBoxStyle>(COMPONENT(QueryCheckBoxStylePrefab))
			.with<CheckBox>()
			.without<Widget>()
			.with(flecs::Prefab)
			.cached().build() });
	};

	void ButtonFeature::CreateObservers(flecs::world& world) {
		world.observer<>("TriggerCheckBoxAction")
			.with<CheckBoxState>(flecs::Wildcard)
			.event(flecs::OnSet)
			.each([&world](flecs::iter& it, size_t t) {
			auto event = it.pair(0);
			it.entity(t).children([&world, &event](flecs::entity action) {
				if (action.has<ECS::Action>() && action.has(event))
					action.enable<ECS::Action>(); });
				});

		world.observer<>("TriggerCheckBoxInverseAction")
			.with<CheckBoxState>(flecs::Wildcard)
			.event(flecs::OnRemove)
			.each([&world](flecs::iter& it, size_t t) {
			auto event = it.pair(0);
			it.entity(t).children([&world, &event](flecs::entity action) {
				if (action.has<ECS::Invert>() && action.has(event))
					action.enable<ECS::Invert>(); });
				});

		world.observer<const WidgetInstance>("ExclusiveRadioButtons")
			.with<Radio>()
			.with(CheckBoxState::Checked)
			.event(flecs::OnSet)
			.each([&world](flecs::entity entity, const WidgetInstance& widget) {
			auto checkBox = StaticCastSharedRef<SCheckBox>(widget.Value.ToSharedRef());
			checkBox->SetIsChecked(ECheckBoxState::Checked);
			checkBox->SetVisibility(EVisibility::HitTestInvisible);
			// Uncheck others
			entity.parent().children([&entity](flecs::entity other) {
				if (other.has(CheckBoxState::Checked) && other.id() != entity.id()) {
					auto checkBox = StaticCastSharedRef<SCheckBox>(other.get<WidgetInstance>()->Value.ToSharedRef());
					checkBox->SetIsChecked(ECheckBoxState::Unchecked);
					checkBox->SetVisibility(EVisibility::Visible);
					other.add(CheckBoxState::Unchecked);
				}});
				});

		//world.observer<const UIScheme>("UpdateButtonStyles")
		//	.term_at(0).singleton()
		//	.event(flecs::OnSet)
		//	.each([&world](const UIScheme& scheme) {
		//	world.get<QueryButtonStylePrefab>()->Value
		//		.each([](flecs::entity prefab, ButtonStyle& style) {
		//		SetButtonStyle(prefab);
		//			});

		//	world.get<QueryCheckBoxStylePrefab>()->Value
		//		.each([](flecs::entity prefab, CheckBoxStyle& style) {
		//		SetCheckBoxStyle(prefab);
		//			});
		//		});
	}
}