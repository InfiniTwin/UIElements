// Fill out your copyright notice in the Description page of Project Settings.


#include "LocalizationFeature.h"
#include "ECS.h"

namespace UIElements {
	void LocalizationFeature::RegisterOpaqueTypes(flecs::world& world) {
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
	}

	void LocalizationFeature::RegisterComponents(flecs::world& world) {
		world.component<Locale>().member<std::string>(MEMBER(Locale::Value));
		world.component<Text>().member<FText>(MEMBER(Text::Value));
		world.component<LocalizedText>()
			.on_add([&world](flecs::entity entity, LocalizedText& lt) { // Localize on creation
			auto value = LoadStringTable(GetStringTablePath(lt.Value, world.get<Locale>()->Value)).find(KEY(lt.Value));
			entity.set<Text>({ FText::FromString(FString(value->second.c_str())) });
		});
	};

	void LocalizationFeature::RegisterSystems(flecs::world& world) {
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
					if (TABLE(localizedText.Value) == stringTableName)
						text.Value = FText::FromString(FString(stringTable.find(KEY(localizedText.Value))->second.c_str()));
				});
			}
		});
	};

	void LocalizationFeature::Initialize(flecs::world& world) {};
}