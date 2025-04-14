// Fill out your copyright notice in the Description page of Project Settings.


#include "TextFeature.h"
#include "ECS.h"

namespace UIElements {
	void TextFeature::RegisterOpaqueTypes(flecs::world& world) {
		// std::string <=> flecs::String
		world.component<std::string>()
			.opaque(flecs::String)
			.serialize([](const flecs::serializer* s, const std::string* data) {
			const char* str = data->c_str();
			return s->value(flecs::String, &str);
		})
			.assign_string([](std::string* data, const char* value) {
			*data = value;
		});

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
	}

	void TextFeature::RegisterComponents(flecs::world& world) {
		world.component<Locale>().member<std::string>(MEMBER(Locale::Value));

		world.component<Text>().member<FText>(MEMBER(Text::Value));

		world.component<LocalizedText>()
			.on_set([&world](flecs::entity entity, LocalizedText& lt) { // Localize on creation
			auto path = GetStringTablePath(lt.Value, world.get<Locale>()->Value);
			auto table = LoadStringTable(path); 
			auto key = Key(lt.Value);
			auto value = table.find(key);
			auto str = value->second.c_str();
			entity.set<Text>({ FText::FromString(FString(str)) });
			auto text = FString(str);
			//auto value = LoadStringTable(GetStringTablePath(lt.Value, world.get<Locale>()->Value)).find(KEY(lt.Value));
			//entity.set<Text>({ FText::FromString(FString(value->second.c_str())) });
		})
			.member<std::string>(MEMBER(LocalizedText::Value));

		world.component<TextBlock>()
			.on_add([](flecs::entity e, TextBlock& tb) { SAssignNew(tb.Value, STextBlock); })
			.on_remove([](flecs::entity e, TextBlock& w) {w.Value.Reset(); });
	};

	void TextFeature::RegisterObservers(flecs::world& world) {
		// Set localized text to STextBlock
		world.observer<Text, const TextBlock>()
			.event(flecs::OnSet)
			.each([](const Text& text, const TextBlock& textBlock) {
			textBlock.Value->SetText(text.Value);
		});

		static flecs::query<Text, const LocalizedText> localizableTexts = world.query_builder<Text, const LocalizedText>().cached().build();

		// Localize all on locale change
		world.observer<Locale>()
			.term_at(0).singleton()
			.event(flecs::OnSet)
			.each([](flecs::iter& iter, size_t i, Locale& locale) {
			auto stringTableNames = Assets::GetFolders(Assets::GetAssetPath("", LocalizationFolder));
			for (const std::string& stringTableName : stringTableNames) {
				auto stringTable = LoadStringTable(GetStringTablePath(stringTableName, locale.Value));
				localizableTexts.each([&stringTableName, &stringTable](Text& text, const LocalizedText& localizedText) {
					if (Table(localizedText.Value) == stringTableName)
						text.Value = FText::FromString(FString(stringTable.find(Key(localizedText.Value))->second.c_str()));
				});
			}
		});
	};

	void TextFeature::Initialize(flecs::world& world) {};
}