// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "flecs.h"
#include "Assets.h"
#include <unordered_map>
#include <fstream>
#include <sstream>

inline constexpr TCHAR LocalizationFolder[] = TEXT("Localization");
constexpr const char* TableKeyDelimiter = "::";
constexpr const TCHAR KeyValueDelimiter = '=';

namespace UIElements {
	struct TextFeature {
		static void RegisterOpaqueTypes(flecs::world& world);
		static void RegisterComponents(flecs::world& world);
		static void RegisterObservers(flecs::world& world);
		static void Initialize(flecs::world& world);

		static inline std::string Table(const std::string& tableKey) {
			size_t pos = tableKey.find(TableKeyDelimiter);
			if (pos != std::string::npos)
				return tableKey.substr(0, pos);
			return tableKey;
		}

		static inline std::string Key(const std::string& tableKey) {
			size_t pos = tableKey.find(TableKeyDelimiter);
			if (pos != std::string::npos) {
				return tableKey.substr(pos + 2);
			}
			return tableKey;
		}

		static inline std::string GetStringTablePath(const std::string& tableKey, const std::string& locale) {
			return TCHAR_TO_UTF8(*Assets::GetAssetPath(TextExtension,
				LocalizationFolder,
				UTF8_TO_TCHAR(Table(tableKey).c_str()),
				UTF8_TO_TCHAR(locale.c_str())
			));
		}

		static inline std::unordered_map<std::string, std::string> LoadStringTable(const std::string& path) {
			std::unordered_map<std::string, std::string> map;
			std::ifstream file(path);

			std::string line;
			while (std::getline(file, line)) {
				size_t delimiterPos = line.find(KeyValueDelimiter);
				if (delimiterPos != std::string::npos)
					map[line.substr(0, delimiterPos)] = line.substr(delimiterPos + 1);
			}

			file.close();
			return map;
		}
	};

	struct Locale { std::string Value; };

	struct LocalizedText { std::string Value; };

	struct Text { FText Value; };

	struct TextBlock { TSharedPtr<STextBlock> Value; };
}