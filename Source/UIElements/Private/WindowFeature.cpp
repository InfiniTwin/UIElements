// Fill out your copyright notice in the Description page of Project Settings.


#include "WindowFeature.h"

namespace UI {
	void WindowFeature::RegisterComponents(flecs::world& world) {
		world.component<Window>().add(flecs::OnInstantiate, flecs::Inherit);
	};

	void WindowFeature::CreateQueries(flecs::world& world) {
	};

	void WindowFeature::CreateSystems(flecs::world& world) {
		world.observer<>("HandleWindowSwitch")
			.with<Window>()
			.with<WidgetState>().second(flecs::Wildcard)
			.event(flecs::OnSet)
			.each([&world](flecs::entity entity) {
			auto checkBox = entity.parent();
			auto checkBoxWidget = StaticCastSharedRef<SCheckBox>(checkBox.get<WidgetInstance>().Value.ToSharedRef());
			if (entity.has(WidgetState::Opened)) {
				OpenWindow(entity);
				checkBoxWidget->SetVisibility(EVisibility::HitTestInvisible);
			}
			else {
				checkBoxWidget->SetIsChecked(ECheckBoxState::Unchecked);
				checkBoxWidget->SetVisibility(EVisibility::Visible);
				checkBox.add(Unchecked);
			}
				});
	}
}