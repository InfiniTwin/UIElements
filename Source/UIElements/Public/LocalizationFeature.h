// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "flecs.h"
#include "Assets.h"
#include <unordered_map>
#include <fstream>
#include <sstream>

inline constexpr TCHAR LocalizationFolder[] = TEXT("Localization");
inline constexpr TCHAR KeyValueDelimiter = '=';

constexpr const char* Table(const char* str) {
	const char* start = str;
	while (*str && !(str[0] == ':' && str[1] == ':'))
		++str;
	return (*str == '\0') ? start : str;
}
#define TABLE(str) Table(str.c_str())

constexpr const char* Key(const char* str) {
	while (*str != '\0' && *str != ':')
		++str;
	if (*str == ':')
		++str;
	return str;
}
#define KEY(str) Key(str.c_str())

namespace UIElements {
	struct LocalizationFeature {
		static void RegisterOpaqueTypes(flecs::world& world);
		static void RegisterComponents(flecs::world& world);
		static void RegisterSystems(flecs::world& world);
		static void Initialize(flecs::world& world);

		static inline std::string GetStringTablePath(const std::string& tableKey, const std::string& locale) {
			return TCHAR_TO_UTF8(*Assets::GetAssetPath(TextExtension,
				LocalizationFolder,
				TABLE(tableKey),
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
}