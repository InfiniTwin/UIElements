// Fill out your copyright notice in the Description page of Project Settings.


#include "TypographyFeature.h"
#include "ECS.h"
#include "Logging/StructuredLog.h"

namespace UIElements {
	void TypographyFeature::RegisterOpaqueTypes(flecs::world& world) {
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

	void TypographyFeature::RegisterComponents(flecs::world& world) {
		using namespace ECS;
		world.component<TextFont>().member<FString>(VALUE).add(flecs::OnInstantiate, flecs::Inherit);
		world.component<FontFace>().member<FString>(VALUE).add(flecs::OnInstantiate, flecs::Inherit);
		world.component<FontSize>().member<int>(VALUE).add(flecs::OnInstantiate, flecs::Inherit);
		world.component<FontInfo>().add(flecs::OnInstantiate, flecs::Inherit);
		world.component<FontSynced>().add(flecs::CanToggle);

		world.component<Locale>().member<FString>(VALUE);
		world.component<LocalizedText>().member<FString>(VALUE);

		world.component<TextBlock>().add(flecs::OnInstantiate, flecs::Inherit);
	};

	void TypographyFeature::CreateQueries(flecs::world& world) {
		world.set(LocalizedTextQuery{ world.query_builder<const LocalizedText, const Widget>("LocalizedTextQuery").cached().build() });

		world.set(TextPrefabQuery{ world.query_builder<const FontFace, const FontSize>("TextPrefabQuery").with(flecs::Prefab).cached().build() });
	};

	void TypographyFeature::CreateObservers(flecs::world& world) {
		world.observer<const TextFont>("UpdateTextPrefabFontInfo")
			.term_at(0).singleton()
			.event(flecs::OnSet)
			.each([&world](const TextFont& tf) {
			world.get<TextPrefabQuery>()->Value.each([&world, &tf](flecs::entity tp, const FontFace& ff, const FontSize& fs) {
				SetFontInfo(tp, tf.Value, ff.Value, fs.Value);
				world.each(world.pair(flecs::IsA, tp), [](flecs::entity i) {
					i.disable<FontSynced>();
					}); });
				});

		world.observer<const LocalizedText, const Widget>("LocalizeTextBlock")
			.with<TextBlock>()
			.event(flecs::OnSet)
			.each([&world](const LocalizedText& lt, const Widget& w) {
			auto table = LoadTable(GetTablePath(lt.Value, world.get<Locale>()->Value));
			StaticCastSharedPtr<STextBlock>(w.Value)->SetText(FText::FromString(*table.Find(GetKey(lt.Value)))); });

		world.observer<const Locale>("LocalizeAllText")
			.term_at(0).singleton()
			.event(flecs::OnSet)
			.each([&world](const Locale& l) {
			auto tableNames = Assets::GetFolders(Assets::GetAssetPath("", LocalizationFolder));
			auto texts = world.get<LocalizedTextQuery>()->Value;
			for (const FString& tableName : tableNames) {
				auto table = LoadTable(GetTablePath(tableName, l.Value));
				texts.each([&tableName, &table](flecs::entity e, const LocalizedText& lt, const Widget& w) {
					if (GetTable(lt.Value) == tableName) {
						FText text = FText::FromString(*table.Find(GetKey(lt.Value)));
						if (e.has<TextBlock>())
							StaticCastSharedPtr<STextBlock>(w.Value)->SetText(text);
					}
					});
			}});
	};

	void TypographyFeature::CreateSystems(flecs::world& world) {
		world.system("AddTextBlockWidget")
			.with<TextBlock>()
			.without<Widget>()
			.each([](flecs::entity e) { e.set(Widget{ SNew(STextBlock) }); });

		world.system("AddFontInfo")
			.with(flecs::Prefab)
			.with<FontFace>()
			.with<FontSize>()
			.without<FontInfo>()
			.each([](flecs::entity e) { e.add<FontInfo>(); });

		world.system("AddFontSynced")
			.with<FontInfo>()
			.without<FontSynced>()
			.each([](flecs::entity e) { e.add<FontSynced>().disable<FontSynced>(); });

		world.system<const FontInfo, const Widget>("SyncTextBlockFont")
			.with<TextBlock>()
			.with<FontSynced>().id_flags(flecs::TOGGLE).without<FontSynced>()
			.each([&world](flecs::entity tb, const FontInfo& fi, const Widget& w) {
			tb.enable<FontSynced>();
			StaticCastSharedPtr<STextBlock>(w.Value)->SetFont(fi.Value); });
	}
}