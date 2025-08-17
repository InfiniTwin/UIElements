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

		world.component<ButtonState>().add(flecs::Exclusive);
		world.component<CheckBoxState>().add(flecs::Exclusive);
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
			.each([&](flecs::iter& it, size_t t) {
			auto event = it.pair(0);
			it.entity(t).children([&](flecs::entity action) {
				if (action.has<ECS::Action>() && action.has(event))
					action.enable<ECS::Action>(); });
				});

		world.observer<>("TriggerCheckBoxInverseAction")
			.with<CheckBoxState>(flecs::Wildcard)
			.event(flecs::OnRemove)
			.each([](flecs::iter& it, size_t t) {
			auto event = it.pair(0);
			it.entity(t).children([&](flecs::entity action) {
				if (action.has<ECS::Invert>() && action.has(event))
					action.enable<ECS::Invert>(); });
				});

		world.observer<const WidgetInstance>("ExclusiveRadioButtons")
			.with<Radio>()
			.with(Checked)
			.event(flecs::OnSet)
			.each([](flecs::entity entity, const WidgetInstance& widget) {
			auto checkBox = StaticCastSharedRef<SCheckBox>(widget.Value.ToSharedRef());
			checkBox->SetIsChecked(ECheckBoxState::Checked);
			entity.add(HitTestInvisible);
			// Uncheck others
			entity.parent().children([&entity](flecs::entity other) {
				if (other.has(Checked) && other.id() != entity.id()) {
					other.add(Visible).add(Unchecked);
					auto checkBox = StaticCastSharedRef<SCheckBox>(other.try_get<WidgetInstance>()->Value.ToSharedRef());
					checkBox->SetIsChecked(ECheckBoxState::Unchecked);
				}});
				});

		world.observer<>("SetCheckBoxWidgetState")
			.with<CheckBoxState>(flecs::Wildcard)
			.with<WidgetInstance>()
			.event(flecs::OnAdd)
			.each([](flecs::entity entity) {
			ECheckBoxState widgetState = ECheckBoxState::Unchecked;
			CheckBoxState state = entity.target<CheckBoxState>().to_constant<CheckBoxState>();

			switch (state) {
			case Unchecked:     widgetState = ECheckBoxState::Unchecked; break;
			case Checked:       widgetState = ECheckBoxState::Checked; break;
			case Undetermined:  widgetState = ECheckBoxState::Undetermined; break;
			}

			StaticCastSharedRef<SCheckBox>(entity.try_get_mut<WidgetInstance>()->Value.ToSharedRef())
				->SetIsChecked(widgetState);
				});
	}
}