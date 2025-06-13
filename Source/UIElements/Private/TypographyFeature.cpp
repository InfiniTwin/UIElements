// Fill out your copyright notice in the Description page of Project Settings.


#include "TypographyFeature.h"
#include "ECS.h"
#include "ColorFeature.h"

namespace UI {
	void TypographyFeature::RegisterComponents(flecs::world& world) {
		using namespace ECS;
		world.component<TextFont>().member<FString>(VALUE);
		world.component<IconFont>().member<FString>(VALUE);

		world.component<FontFace>().member<FString>(VALUE).add(flecs::OnInstantiate, flecs::Inherit);
		world.component<FontSize>().member<int>(VALUE).add(flecs::OnInstantiate, flecs::Inherit);
		world.component<FontInfo>().add(flecs::OnInstantiate, flecs::Inherit);

		world.component<Locale>().member<FString>(VALUE);
		world.component<LocalizedText>().member<FString>(VALUE).add(flecs::OnInstantiate, flecs::Override);

		world.component<Icon>().member<FString>(VALUE);

		world.component<TextBlock>();
	};

	void TypographyFeature::CreateQueries(flecs::world& world) {
		world.component<QueryText>();
		world.set(QueryText{
			world.query_builder<const FontFace, const FontSize>(COMPONENT(QueryText))
			.with<FontInfo>()
			.with<LocalizedText>()
			.with(flecs::Prefab).cached().build() });

		world.component<QueryLocalizedText>();
		world.set(QueryLocalizedText{
			world.query_builder<const LocalizedText, const WidgetInstance>(COMPONENT(QueryLocalizedText))
			.cached().build() });

		world.component<QueryIcon>();
		world.set(QueryIcon{
			world.query_builder<const FontFace, const FontSize>(COMPONENT(QueryIcon))
			.with<FontInfo>()
			.with<Icon>()
			.with(flecs::Prefab).cached().build() });
	};

	void TypographyFeature::CreateObservers(flecs::world& world) {
		world.observer<const TextFont>("SetTextPrefabFontInfo")
			.term_at(0).singleton()
			.event(flecs::OnSet)
			.each([&world](const TextFont& f) {
			world.get<QueryText>()->Value
				.each([&f](flecs::entity p, const FontFace& ff, const FontSize& fs) {
				SetFontInfo(p, f.Value, ff.Value, fs.Value);
					});
				});

		world.observer<const IconFont>("SetIconPrefabFontInfo")
			.term_at(0).singleton()
			.event(flecs::OnSet)
			.each([&world](const IconFont& f) {
			world.get<QueryIcon>()->Value
				.each([&f](flecs::entity p, const FontFace& ff, const FontSize& fs) {
				SetFontInfo(p, f.Value, ff.Value, fs.Value);
					});
				});

		world.observer<const FontInfo>("SetInstanceFontInfo")
			.with(flecs::Prefab)
			.event(flecs::OnSet)
			.each([&world](flecs::entity prefab, const FontInfo& fi) {
			TArray<flecs::entity> instances;
			ECS::GetInstances(world, prefab, instances);
			for (flecs::entity instance : instances)
			{
				if (!instance.has<WidgetInstance>())
					return;
				TSharedPtr<SWidget> widget = instance.get_mut<WidgetInstance>()->Value;
				if (instance.has<TextBlock>())
					SetTextBlockFontInfo(widget, fi.Value);
			}
				});

		world.observer<const WidgetInstance, const Icon>("SetIcon")
			.event(flecs::OnSet)
			.each([&world](flecs::entity e, const WidgetInstance& w, const Icon& code) {
			int32 codePoint = FParse::HexNumber(*code.Value);
			FString unicodeChar;
			unicodeChar.AppendChar(static_cast<TCHAR>(codePoint));
			SetTextBlockText(w.Value, unicodeChar); });

		world.observer<const WidgetInstance, const LocalizedText>("LocalizeText")
			.event(flecs::OnSet)
			.each([&world](flecs::entity e, const WidgetInstance& w, const LocalizedText& lt) {
			if (lt.Value.IsEmpty())
				return;
			FString text = GetLocalizedText(world.get<Locale>()->Value, lt.Value);
			if (e.has<TextBlock>())
				SetTextBlockText(w.Value, text); });

		world.observer<const Locale>("LocalizeAllText")
			.term_at(0).singleton()
			.event(flecs::OnSet)
			.each([&world](const Locale& locale) {
			auto tableNames = Assets::GetFolders(Assets::GetAssetPath("", LocalizationFolder));
			TMap<FString, TMap<FString, FString>> tables;
			for (const FString& tableName : tableNames)
				tables.Add(tableName, LoadTable(GetTablePath(tableName, locale.Value)));
			world.get<QueryLocalizedText>()->Value
				.each([&](flecs::entity e, const LocalizedText& lt, const WidgetInstance& w) {
				if (const auto* table = tables.Find(GetTable(lt.Value)); table)
					if (const auto* result = table->Find(GetKey(lt.Value)))
					{
						TSharedPtr<SWidget> widget = e.get_mut<WidgetInstance>()->Value;
						if (e.has<TextBlock>())
							SetTextBlockText(widget, *result);
					} });
				});
	};
}