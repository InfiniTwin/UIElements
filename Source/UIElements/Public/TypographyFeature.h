// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "flecs.h"
#include "Assets.h"
#include "Containers/UnrealString.h"
#include "WidgetFeature.h"

namespace UIElements {
	inline constexpr TCHAR LocalizationFolder[] = TEXT("Localization");
	constexpr const char* TableKeyDelimiter = "::";
	constexpr const TCHAR KeyValueDelimiter[] = TEXT("=");

	struct Font { FString Value; };
	struct FontFace { FString Value; };
	struct FontSize { int Value; };
	struct FontSet {};

	struct TextBlock {};

	struct Icon { FString Value; };
	struct IconFont { FSlateFontInfo Value; };

	struct Locale { FString Value; };
	struct LocalizedText { FString Value; };

	struct LocalizedTextQuery { flecs::query<const LocalizedText, const Widget> Value; };

	struct TypographyFeature {
		static void RegisterOpaqueTypes(flecs::world& world);
		static void RegisterComponents(flecs::world& world);
		static void CreateQueries(flecs::world& world);
		static void CreateObservers(flecs::world& world);
		static void CreateSystems(flecs::world& world);
	};

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

	inline TMap<FString, FString> LoadTable(const FString& path)
	{
		TMap<FString, FString> map;
		FString fileContents;
		if (FFileHelper::LoadFileToString(fileContents, *path))
		{
			TArray<FString> lines;
			fileContents.ParseIntoArrayLines(lines);
			for (const FString& line : lines)
			{
				FString key, value;
				if (line.Split(KeyValueDelimiter, &key, &value))
					map.Add(key, value);
			}
		}
		return map;
	}
}