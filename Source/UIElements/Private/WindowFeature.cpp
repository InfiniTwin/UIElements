// Fill out your copyright notice in the Description page of Project Settings.


#include "WindowFeature.h"
#include "ECS.h"
#include "Logging/LogMacros.h"

namespace UI {
#if WITH_EDITORONLY_DATA
	static FDelegateHandle PIEHandle;
#endif

	void WindowFeature::CreateComponents(flecs::world& world) {
		using namespace ECS;
		world.component<Window>().add(flecs::OnInstantiate, flecs::Inherit);
		world.component<WindowTitle>().add(flecs::OnInstantiate, flecs::Inherit);
		world.component<Size>().member<FVector2D>(VALUE).add(flecs::OnInstantiate, flecs::Inherit);
	};

	void WindowFeature::CreateQueries(flecs::world& world) {
		world.component<QueryWindows>();
		world.set(QueryWindows{
			world.query_builder<WidgetInstance>(COMPONENT(QueryWindows))
			.with<Window>()
			.build() });
	};

	void WindowFeature::CreateObservers(flecs::world& world) {
		world.observer<const Size>("OpenWindow")
			.with(Opened)
			.with<Window>()
			.with<WidgetState>().second(flecs::Wildcard)
			.event(flecs::OnSet)
			.each([](flecs::entity window, const Size& size) {
			TSharedRef<SWindow> widget = SNew(SWindow).ClientSize(size.Value);
			widget->SetOnWindowClosed(FOnWindowClosed::CreateLambda([window](const TSharedRef<SWindow>& closedWindow) {
				window.add(Closed);
				}));
			window.set(WidgetInstance{ widget });
			FSlateApplication::Get().AddWindow(widget);
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
		FCoreDelegates::GetApplicationWillTerminateDelegate().AddLambda(closeWindows);
	}
}