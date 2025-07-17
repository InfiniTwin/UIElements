// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "flecs.h"
#include "Assets.h"
#include "Containers/UnrealString.h"
#include "WidgetFeature.h"
#include "WindowFeature.h"

namespace UI {
	struct TypographyFeature {
		static void RegisterComponents(flecs::world& world);
		static void CreateQueries(flecs::world& world);
		static void CreateObservers(flecs::world& world);
	};

	inline constexpr TCHAR LocalizationFolder[] = TEXT("Localization");
	constexpr const char* TableKeyDelimiter = "::";
	constexpr const TCHAR KeyValueDelimiter[] = TEXT("=");

	struct TextFont { FString Value; };
	struct IconFont { FString Value; };

	struct FontFace { FString Value; };
	struct FontSize { int Value; };
	struct FontInfo { FSlateFontInfo Value; };

	struct Locale { FString Value; };
	struct Localized { FString Value; };
	struct Text { FString Value; };

	struct Icon { FString Value; };

	struct TextBlock {};

	struct QueryText { flecs::query<const FontFace, const FontSize> Value; };
	struct QueryLocalizedText { flecs::query<const Text, const WidgetInstance> Value; };

	struct QueryIcon { flecs::query<const FontFace, const FontSize> Value; };

	inline FString GetTable(const FString& tableKey) {
		FString tableKeyString(tableKey);
		int32 pos = tableKeyString.Find(TableKeyDelimiter);
		if (pos != INDEX_NONE)
			return tableKeyString.Left(pos);
		return tableKeyString;
	}

	inline FString GetKey(const FString& tableKey) {
		FString tableKeyString(tableKey);
		int32 pos = tableKeyString.Find(TableKeyDelimiter);
		if (pos != INDEX_NONE)
			return tableKeyString.RightChop(pos + 2);
		return tableKeyString;
	}

	inline FString GetTablePath(const FString& tableKey, const FString locale) {
		using namespace Assets;
		return GetAssetPath(TextExtension,
			LocalizationFolder,
			*GetTable(tableKey),
			*FString(locale)
		);
	}

	inline TMap<FString, FString> LoadTable(const FString& path) {
		TMap<FString, FString> map;
		FString fileContents;
		if (FFileHelper::LoadFileToString(fileContents, *path)) {
			TArray<FString> lines;
			fileContents.ParseIntoArrayLines(lines);
			for (const FString& line : lines) {
				FString key, value;
				if (line.Split(KeyValueDelimiter, &key, &value))
					map.Add(key, value);
			}
		}
		return map;
	}

	static inline void SetFontInfo(const flecs::entity e, const FString& name, const FString& fontFace, int32 fontSize) {
		const FString path = FPaths::ProjectContentDir() / TEXT("Slate/Fonts/") + name + TEXT("-") + fontFace + TEXT(".ttf");
		e.set<FontInfo>({ FSlateFontInfo(path, fontSize) });
	}

	static inline void SetTextBlockFontInfo(const TSharedPtr<SWidget>& widget, const FSlateFontInfo& fontInfo) {
		StaticCastSharedPtr<STextBlock>(widget)->SetFont(fontInfo);
	}

	UIELEMENTS_API inline FString GetLocalizedText(flecs::world& world, const FString& localizedText) {
		return *LoadTable(GetTablePath(localizedText, world.try_get<Locale>()->Value)).Find(GetKey(localizedText));
	}

	static inline void SetText(flecs::entity entity, const TSharedPtr<SWidget>& widget, const FString& text) {
		if (entity.has<TextBlock>())
			StaticCastSharedPtr<STextBlock>(widget)->SetText(FText::FromString(text));
		else if (entity.has<WindowTitle>())
			SetWindowTitle(widget, text);
	}

	static inline void AddTextBlockWidget(flecs::entity entity) {
		TSharedRef<STextBlock> widget = SNew(STextBlock);
		SetTextBlockFontInfo(widget, entity.try_get<FontInfo>()->Value);
		entity.set(WidgetInstance{ widget });
	}
}