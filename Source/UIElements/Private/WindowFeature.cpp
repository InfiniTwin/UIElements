// Fill out your copyright notice in the Description page of Project Settings.


#include "WindowFeature.h"

namespace UI {
#if WITH_EDITORONLY_DATA
	static FDelegateHandle PIEHandle;
#endif

	void WindowFeature::RegisterComponents(flecs::world& world) {
		world.component<Window>().add(flecs::OnInstantiate, flecs::Inherit);
		world.component<WindowTitle>().add(flecs::OnInstantiate, flecs::Inherit);
		world.component<WindowStyle>().add(flecs::OnInstantiate, flecs::Inherit);
	};

	void WindowFeature::CreateQueries(flecs::world& world) {
		world.component<QueryWindows>();
		world.set(QueryWindows{
			world.query_builder<WidgetInstance>(COMPONENT(QueryWindows))
			.with<Window>()
			.build() });

		world.component<QueryWindowStylePrefab>();
		world.set(QueryWindowStylePrefab{
			world.query_builder<WindowStyle>(COMPONENT(QueryWindowStylePrefab))
			.with<Window>()
			.without<Widget>()
			.with(flecs::Prefab)
			.cached().build() });
	};

		world.observer<>("OpenWindow")
	void WindowFeature::CreateObservers(flecs::world& world) {
			.with<Window>()
			.with<WidgetState>().second(flecs::Wildcard)
			.event(flecs::OnSet)
			.each([&world](flecs::entity window) {
			if (window.has(WidgetState::Opened))
				OpenWindow(window);
		});
	}

	void WindowFeature::Initialize(flecs::world& world) {
		auto closeWindows = [&world] {
			if (!world) return;
			world.try_get<QueryWindows>()->Value
				.each([](const WidgetInstance& widget) {
				TSharedPtr<SWindow> window = StaticCastSharedPtr<SWindow>(widget.Value);
				if (window.IsValid()) {
					window->RequestDestroyWindow();
				}
			});
		};
#if WITH_EDITORONLY_DATA
		PIEHandle = FEditorDelegates::EndPIE.AddLambda([closeWindows](bool) {
			closeWindows();
			if (PIEHandle.IsValid()) {
				FEditorDelegates::EndPIE.Remove(PIEHandle);
				PIEHandle.Reset();
			}
		});
#endif
		FCoreDelegates::ApplicationWillTerminateDelegate.AddLambda(closeWindows);
	}
}