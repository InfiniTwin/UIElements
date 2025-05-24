// Fill out your copyright notice in the Description page of Project Settings.


#include "TypographyFeature.h"
#include "ECS.h"
#include "ColorFeature.h"
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
		world.component<FontInfo>().member<FSlateFontInfo>(VALUE).add(flecs::OnInstantiate, flecs::Inherit);

		world.component<Locale>().member<FString>(VALUE);
		world.component<LocalizedText>().member<FString>(VALUE);

		world.component<TextBlock>().add(flecs::OnInstantiate, flecs::Inherit);
	};

	void TypographyFeature::CreateQueries(flecs::world& world) {
		world.set(LocalizedTextQuery{
			world.query_builder<const LocalizedText, const Widget>(COMPONENT(LocalizedTextQuery))
			.cached().build() });

		world.set(TextPrefabQuery{
			world.query_builder<const FontInfo, const FontFace, const FontSize>(COMPONENT(TextPrefabQuery))
			.with(flecs::Prefab).cached().build() });
	};

	void TypographyFeature::CreateObservers(flecs::world& world) {
		world.observer<const TextFont>("SetPrefabFontInfo")
			.term_at(0).singleton()
			.event(flecs::OnSet)
			.each([&world](const TextFont& tf) {
			world.get<TextPrefabQuery>()->Value
				.each([&world, &tf](flecs::entity tp, FontInfo, const FontFace& ff, const FontSize& fs) {
				SetFontInfo(tp, tf.Value, ff.Value, fs.Value);
					});
				});

		world.observer<const FontInfo>("SetInstanceFontInfo")
			.with(flecs::Prefab)
			.event(flecs::OnSet)
			.each([&world](flecs::entity p, const FontInfo& fi) {
			auto pp = p.path();
			world.each(world.pair(flecs::IsA, p), [&fi](flecs::entity i) {
				if (!i.has<Widget>())
					return;
				TSharedPtr<SWidget> widget = i.get_mut<Widget>()->Value;
				if (i.has<TextBlock>())
					SetTextBlockFontInfo(widget, fi.Value); });
				});

		world.observer<const Locale>("LocalizeAllText")
			.term_at(0).singleton()
			.event(flecs::OnSet)
			.each([&world](const Locale& l) {
			auto tableNames = Assets::GetFolders(Assets::GetAssetPath("", LocalizationFolder));
			TMap<FString, TMap<FString, FString>> tables;
			for (const FString& tableName : tableNames)
				tables.Add(tableName, LoadTable(GetTablePath(tableName, l.Value)));
			world.get<LocalizedTextQuery>()->Value
				.each([&](flecs::entity e, const LocalizedText& lt, const Widget& w) {
				if (const auto* table = tables.Find(GetTable(lt.Value)); table)
					if (const auto* result = table->Find(GetKey(lt.Value)))
						if (e.has<TextBlock>())
							StaticCastSharedPtr<STextBlock>(e.get_mut<Widget>()->Value)->SetText(FText::FromString(*result));
					}); });
	};

	void TypographyFeature::CreateSystems(flecs::world& world) {
		world.system("AddTextBlockWidget")
			.without<Widget>()
			.with<TextBlock>()
			.each([&world](flecs::entity e) {
			auto widget = SNew(STextBlock);
			SetTextBlockColor(widget, e.get<Color>()->Value);
			SetTextBlockFontInfo(widget, e.get<FontInfo>()->Value);
			LocalizeTextBlock(world, widget, e.get<LocalizedText>()->Value);
			e.set(Widget{ widget }); });
	}
}