// Fill out your copyright notice in the Description page of Project Settings.


#include "TextFeature.h"
#include "ECS.h"
#include "Logging/StructuredLog.h"

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
		world.component<Font>().member<FString>(VALUE).add(flecs::OnInstantiate, flecs::Inherit);
		world.component<FontFace>().member<FString>(VALUE).add(flecs::OnInstantiate, flecs::Inherit);
		world.component<FontSize>().member<int>(VALUE).add(flecs::OnInstantiate, flecs::Inherit);
		world.component<FontInfo>().add(flecs::CanToggle);

		world.component<Locale>().member<FString>(VALUE);
		world.component<LocalizedText>().member<FString>(VALUE);

		world.component<TextBlock>().add(flecs::OnInstantiate, flecs::Inherit);
	};

	void TextFeature::CreateQueries(flecs::world& world) {
		world.set(LocalizedTextQuery{ world.query_builder<const LocalizedText, const Widget>().cached().build() });
	};

	void TextFeature::CreateObservers(flecs::world& world) {
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
					if (GetTable(lt.Value) == tableName)
						if (e.has<TextBlock>())
							StaticCastSharedPtr<STextBlock>(w.Value)->SetText(FText::FromString(*table.Find(GetKey(lt.Value))));
					});
			}});
	};

	void TextFeature::CreateSystems(flecs::world& world) {
		world.system("AddWidgetToTextBlock")
			.with<TextBlock>()
			.without<Widget>()
			.each([](flecs::entity e) { e.set(Widget{ SNew(STextBlock) }); });

		world.system("AddFontInfo")
			.with(flecs::Prefab)
			.with<Font>()
			.with<FontFace>()
			.with<FontSize>()
			.without<FontInfo>()
			.each([](flecs::entity e) { e.add<FontInfo>().disable<FontInfo>(); });

		world.system<const Font, const FontFace, const FontSize>("UpdateFontInfo")
			.with(flecs::Prefab)
			.with<FontInfo>().id_flags(flecs::TOGGLE).without<FontInfo>()
			.each([&world](flecs::entity e, const Font& f, const FontFace& fw, const FontSize& fs) {
			e.enable<FontInfo>();
			const FString fp = FPaths::ProjectContentDir() / TEXT("Slate/Fonts/") + f.Value + TEXT("-") + fw.Value + TEXT(".ttf");
			FSlateFontInfo fi = FSlateFontInfo(fp, fs.Value);
			world.each(world.pair(flecs::IsA, e), [&fi](flecs::entity i) {
				if (i.has<TextBlock>())
					StaticCastSharedPtr<STextBlock>(i.get_mut<Widget>()->Value)->SetFont(fi);
				}); });
	}
}