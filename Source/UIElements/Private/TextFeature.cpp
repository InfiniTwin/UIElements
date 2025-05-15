// Fill out your copyright notice in the Description page of Project Settings.


#include "TextFeature.h"
#include "ECS.h"

namespace UIElements {
	void TextFeature::RegisterOpaqueTypes(flecs::world& world) {
		// FString <=> flecs::String
		world.component<FString>()
			.opaque(flecs::String)
			.serialize([](const flecs::serializer* s, const FString* data) {
			const char* str = TCHAR_TO_UTF8(**data);
			return s->value(flecs::String, &str);
				})
			.assign_string([](FString* data, const char* value) {
			*data = UTF8_TO_TCHAR(value);
				});

		// FText <=> flecs::String
		world.component<FText>()
			.opaque(flecs::String)
			.serialize([](const flecs::serializer* s, const FText* data) {
			FString temp = data->ToString();
			const char* str = TCHAR_TO_UTF8(*temp);
			return s->value(flecs::String, &str);
				})
			.assign_string([](FText* data, const char* value) {
			*data = FText::FromString(UTF8_TO_TCHAR(value));
				});
	}

	void TextFeature::RegisterComponents(flecs::world& world) {
		world.component<Locale>().member<FString>(VALUE);
		world.component<LocalizedText>().member<FString>(VALUE);

		world.component<TextBlock>()
			.add(flecs::OnInstantiate, flecs::Inherit);

		world.component<LabelSmall>()
			.add(flecs::OnInstantiate, flecs::Inherit);
	};

	void TextFeature::CreateQueries(flecs::world& world) {
		world.set(TextBlocksQuery{
			world.query_builder<const TextBlock, const LocalizedText, const Widget>()
			.cached().build()
			});
	};

	void TextFeature::CreateObservers(flecs::world& world) {
		// Localize TextBlock on creattion
		world.observer<const LocalizedText, const Widget>()
			.with<TextBlock>()
			.event(flecs::OnSet)
			.each([&world](const LocalizedText& lt, const Widget& w) {
			auto table = LoadTable(GetTablePath(lt.Value, world.get<Locale>()->Value));
			StaticCastSharedPtr<STextBlock>(w.Value)
				->SetText(FText::FromString(*table.Find(GetKey(lt.Value))));
				});

		// Localize all on locale change
		world.observer<const Locale>()
			.term_at(0).singleton()
			.event(flecs::OnSet)
			.each([&world](const Locale& locale) {
			auto tableNames = Assets::GetFolders(Assets::GetAssetPath("", LocalizationFolder));
			auto textBlocks = world.get<TextBlocksQuery>()->Value;
			for (const FString& tableName : tableNames) {
				auto table = LoadTable(GetTablePath(tableName, locale.Value));
				textBlocks.each([&tableName, &table](const TextBlock& tb, const LocalizedText& lt, const Widget& w) {
					if (GetTable(lt.Value) == tableName)
						StaticCastSharedPtr<STextBlock>(w.Value)
						->SetText(FText::FromString(*table.Find(GetKey(lt.Value))));
					});
			}
				});
	};

	void TextFeature::CreateSystems(flecs::world& world) {
		world.system("AddWidgetToTextBlock")
			.with<TextBlock>()
			.without<Widget>()
			.each([](flecs::entity e) { e.set(Widget{ SNew(STextBlock) }); });
	}
}