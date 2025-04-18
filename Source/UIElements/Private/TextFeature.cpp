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
		world.component<Locale>().member<FString>(MEMBER(Locale::Value));

		world.component<LocalizedText>().member<FString>(MEMBER(LocalizedText::Value));

		world.component<TextBlock>()
			.on_add([](flecs::entity e, TextBlock& tb) { SAssignNew(tb.Value, STextBlock); })
			.on_remove([](flecs::entity e, TextBlock& w) {w.Value.Reset(); });
	};

	void TextFeature::RegisterObservers(flecs::world& world) {
		// Localize STextBlock on creattion
		world.observer<const LocalizedText, const TextBlock>()
			.event(flecs::OnSet)
			.each([&world](const LocalizedText& localizedText, const TextBlock& textBlock) {
			auto table = LoadTable(GetTablePath(localizedText.Value, world.get<Locale>()->Value));
			textBlock.Value->SetText(FText::FromString(*table.Find(GetKey(localizedText.Value))));
				});

		static auto textBlocks = world.query_builder<const LocalizedText, const TextBlock>().cached().build();

		// Localize all on locale change
		world.observer<Locale>()
			.term_at(0).singleton()
			.event(flecs::OnSet)
			.each([](flecs::iter& iter, size_t i, Locale& locale) {
			auto tableNames = Assets::GetFolders(Assets::GetAssetPath("", LocalizationFolder));
			for (const FString& tableName : tableNames) {
				auto table = LoadTable(GetTablePath(tableName, locale.Value));
				textBlocks.each([&tableName, &table](const LocalizedText& localizedText, const TextBlock& textBlock) {
					if (GetTable(localizedText.Value) == tableName)
						textBlock.Value->SetText(FText::FromString(*table.Find(GetKey(localizedText.Value))));
					});
			}
				});
	};

	void TextFeature::Initialize(flecs::world& world) {};
}