// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "flecs.h"
#include "Assets.h"
#include "Containers/UnrealString.h"
#include "WidgetFeature.h"

inline constexpr TCHAR LocalizationFolder[] = TEXT("Localization");
constexpr const char* TableKeyDelimiter = "::";
constexpr const TCHAR KeyValueDelimiter[] = TEXT("=");

namespace UIElements {
	struct Locale { FString Value; };
	struct LocalizedText { FString Value; };

	struct LabelSmall { FSlateFontInfo Value; };	
	struct TextBlock {};

	struct Icon{ FString Value; };
	struct IconFont { FSlateFontInfo Value; };

	struct TextBlocksQuery { flecs::query<const LocalizedText, const TextBlock, const Widget> Value; };

	struct TextFeature {
		static void RegisterOpaqueTypes(flecs::world& world);
		static void RegisterComponents(flecs::world& world);
		static void CreateQueries(flecs::world& world);
		static void RegisterObservers(flecs::world& world);

		static inline FString GetTable(const FString& tableKey) {
			FString tableKeyString(tableKey);
			int32 pos = tableKeyString.Find(TableKeyDelimiter);
			if (pos != INDEX_NONE)
				return tableKeyString.Left(pos);
			return tableKeyString;
		}

		static inline FString GetKey(const FString& tableKey) {
			FString tableKeyString(tableKey);
			int32 pos = tableKeyString.Find(TableKeyDelimiter);
			if (pos != INDEX_NONE)
				return tableKeyString.RightChop(pos + 2);
			return tableKeyString;
		}

		static inline FString GetTablePath(const FString& tableKey, const FString locale) {
			return Assets::GetAssetPath(TextExtension,
				LocalizationFolder,
				*GetTable(tableKey),
				*FString(locale)
			);
		}

		static inline TMap<FString, FString> LoadTable(const FString& path)
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
	};
}