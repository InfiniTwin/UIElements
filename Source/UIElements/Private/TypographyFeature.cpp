// Fill out your copyright notice in the Description page of Project Settings.


#include "TypographyFeature.h"
#include "ECS.h"

namespace UI {
	void TypographyFeature::RegisterComponents(flecs::world& world) {
		using namespace ECS;
		world.component<TextFont>().member<FString>(VALUE).add(flecs::Singleton);
		world.component<IconFont>().member<FString>(VALUE).add(flecs::Singleton);

		world.component<FontFace>().member<FString>(VALUE).add(flecs::OnInstantiate, flecs::Inherit);
		world.component<FontSize>().member<int>(VALUE).add(flecs::OnInstantiate, flecs::Inherit);
		world.component<FontInfo>().add(flecs::OnInstantiate, flecs::Inherit);

		world.component<TextBlock>();

		world.component<Locale>().member<FString>(VALUE).add(flecs::Singleton);
		world.component<Localized>().add(flecs::OnInstantiate, flecs::Inherit);

		world.component<Text>().member<FString>(VALUE);
		world.component<Icon>().member<FString>(VALUE);
	};

	void TypographyFeature::CreateQueries(flecs::world& world) {
		world.component<QueryText>();
		world.set(QueryText{
			world.query_builder<const FontFace, const FontSize>(COMPONENT(QueryText))
			.with<FontInfo>()
			.with<Text>()
			.with(flecs::Prefab).cached().build() });

		world.component<QueryLocalizedText>();
		world.set(QueryLocalizedText{
			world.query_builder<const Text, const WidgetInstance>(COMPONENT(QueryLocalizedText))
			.with<Localized>()
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
			.event(flecs::OnSet)
			.each([&world](const TextFont& f) {
			world.try_get<QueryText>()->Value
				.each([&f](flecs::entity p, const FontFace& ff, const FontSize& fs) {
				SetFontInfo(p, f.Value, ff.Value, fs.Value);
			});
		});

		world.observer<const IconFont>("SetIconPrefabFontInfo")
			.event(flecs::OnSet)
			.each([&world](const IconFont& f) {
			world.try_get<QueryIcon>()->Value
				.each([&world, &f](flecs::entity p, const FontFace& ff, const FontSize& fs) {
				SetFontInfo(p, f.Value, ff.Value, fs.Value);
			});
		});

		world.observer<const FontInfo>("SetInstanceFontInfo")
			.with(flecs::Prefab)
			.event(flecs::OnSet)
			.each([&world](flecs::entity prefab, const FontInfo& fi) {
			TArray<flecs::entity> instances;
			ECS::GetInstances(world, prefab, instances);
			for (flecs::entity instance : instances) {
				if (!instance.has<WidgetInstance>())
					return;
				TSharedPtr<SWidget> widget = instance.try_get_mut<WidgetInstance>()->Value;
				if (instance.has<TextBlock>())
					SetTextBlockFontInfo(widget, fi.Value);
			}
		});

		world.observer<const WidgetInstance, const Icon>("SetIcon")
			.event(flecs::OnSet)
			.each([](flecs::entity entity, const WidgetInstance& widget, const Icon& code) {
			int32 codePoint = FParse::HexNumber(*code.Value);
			FString text;
			text.AppendChar(static_cast<TCHAR>(codePoint));
			SetText(entity, widget.Value, text); });

		world.observer<const WidgetInstance, const Text>("SetText")
			.event(flecs::OnSet)
			.each([&world](flecs::entity entity, const WidgetInstance& widget, const Text& t) {
			FString text = t.Value;
			if (text.IsEmpty())
				return;
			if (entity.has<Localized>())
				text = GetLocalizedText(world, text);
			SetText(entity, widget.Value, text);
		});

		world.observer<const Locale>("LocalizeText")
			.event(flecs::OnSet)
			.each([&world](const Locale& locale) {
			TMap<FString, TMap<FString, FString>> tables;
			for (const FString& tableName : Assets::GetFolders(Assets::GetAssetPath("", LocalizationFolder)))
				tables.Add(tableName, LoadTable(GetTablePath(tableName, locale.Value)));
			world.try_get<QueryLocalizedText>()->Value
				.each([&tables](flecs::entity entity, const Text& lt, const WidgetInstance& w) {
				if (const auto* table = tables.Find(GetTable(lt.Value)); table)
					if (const auto* result = table->Find(GetKey(lt.Value))) {
						TSharedPtr<SWidget> widget = entity.try_get_mut<WidgetInstance>()->Value;
						SetText(entity, widget, *result);
					} });
		});
	};
}