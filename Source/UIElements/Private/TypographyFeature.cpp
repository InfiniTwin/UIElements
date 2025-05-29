﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "TypographyFeature.h"
#include "ECS.h"
#include "ColorFeature.h"

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
		world.component<TextFont>().member<FString>(VALUE);
		world.component<IconFont>().member<FString>(VALUE);

		world.component<FontFace>().member<FString>(VALUE).add(flecs::OnInstantiate, flecs::Inherit);
		world.component<FontSize>().member<int>(VALUE).add(flecs::OnInstantiate, flecs::Inherit);
		world.component<FontInfo>().add(flecs::OnInstantiate, flecs::Inherit);

		world.component<Locale>().member<FString>(VALUE);
		world.component<LocalizedText>().member<FString>(VALUE).add(flecs::OnInstantiate, flecs::Override);

		world.component<Icon>().member<FString>(VALUE).add(flecs::OnInstantiate, flecs::Inherit);

		world.component<TextBlock>().add(flecs::OnInstantiate, flecs::Inherit);
	};

	void TypographyFeature::CreateQueries(flecs::world& world) {
		world.set(TextQuery{
			world.query_builder<const FontFace, const FontSize>(COMPONENT(TextQuery))
			.with<FontInfo>()
			.with<LocalizedText>()
			.with(flecs::Prefab).cached().build() });

		world.set(LocalizedTextQuery{
			world.query_builder<const LocalizedText, const Widget>(COMPONENT(LocalizedTextQuery))
			.cached().build() });

		world.set(IconQuery{
			world.query_builder<const FontFace, const FontSize>(COMPONENT(IconQuery))
			.with<FontInfo>()
			.with<Icon>()
			.with(flecs::Prefab).cached().build() });
	};

	void TypographyFeature::CreateObservers(flecs::world& world) {
		world.observer<const TextFont>("SetTextPrefabFontInfo")
			.term_at(0).singleton()
			.event(flecs::OnSet)
			.each([&world](const TextFont& f) {
			world.get<TextQuery>()->Value
				.each([&f](flecs::entity p, const FontFace& ff, const FontSize& fs) {
				SetFontInfo(p, f.Value, ff.Value, fs.Value);
					});
				});

		world.observer<const IconFont>("SetIconPrefabFontInfo")
			.term_at(0).singleton()
			.event(flecs::OnSet)
			.each([&world](const IconFont& f) {
			world.get<IconQuery>()->Value
				.each([&f](flecs::entity p, const FontFace& ff, const FontSize& fs) {
				SetFontInfo(p, f.Value, ff.Value, fs.Value);
					});
				});

		world.observer<const Widget, const LocalizedText>("LocalizeTextOnCreation")
			.event(flecs::OnSet)
			.each([&world](flecs::entity e, const Widget& w, const LocalizedText& l) {
			if (l.Value.IsEmpty())
				return;
			FString text = GetLocalizedText(world.get<Locale>()->Value, l.Value);
			if (e.has<TextBlock>())
				SetTextBlockText(w.Value, text); });

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
					{
						TSharedPtr<SWidget> widget = e.get_mut<Widget>()->Value;
						if (e.has<TextBlock>())
							SetTextBlockText(widget, *result);
					} });
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

		world.observer<const Widget, const Icon>("SetIcon")
			.event(flecs::OnSet)
			.each([&world](flecs::entity e, const Widget& w, const Icon& i) {
			SetTextBlockText(w.Value, i.Value); });
	};

	void TypographyFeature::CreateSystems(flecs::world& world) {
		//world.system("SetupTextBlockWidget")
		//	.without<Widget>()
		//	.with<TextBlock>()
		//	.each([](flecs::entity e) {
		//	auto widget = SNew(STextBlock);
		//	SetTextBlockColor(widget, e.get<Color>()->Value);
		//	SetTextBlockFontInfo(widget, e.get<FontInfo>()->Value);
		//	e.set(Widget{ widget }); });
	}
}