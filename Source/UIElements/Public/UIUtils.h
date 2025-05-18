// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "utils/utils.h"
#include <string>
#include <sstream>
#include <iomanip>

using namespace material_color_utilities;
struct UIUtils {
public:
	static inline std::string HexFromArgb(Argb argb) {
		std::stringstream ss;
		ss << "0x" << std::setw(8) << std::setfill('0') << std::hex << argb;
		return ss.str();
	}

	static inline material_color_utilities::Argb ArgbFromHex(const std::string& hex) {
		if ((hex.length() == 9 && hex[0] == '#') || (hex.length() == 10 && hex.substr(0, 2) == "0x")) {
			return static_cast<Argb>(std::stoul(hex.substr(hex[0] == '#' ? 1 : 2), nullptr, 16));
		}
		return 0xffffffff;
	}

	static inline std::string HexFromLinearColor(const FLinearColor& color)
	{
		FColor srgb = color.ToFColor(true);
		std::stringstream ss;
		ss << "0x"
			<< std::hex << std::setw(2) << std::setfill('0') << (int)srgb.A
			<< std::setw(2) << (int)srgb.R
			<< std::setw(2) << (int)srgb.G
			<< std::setw(2) << (int)srgb.B;
		return ss.str();
	}

	static inline FLinearColor LinearColorFromHex(const std::string& hex)
	{
		uint32_t argb = 0xffffffff;
		if ((hex.length() == 9 && hex[0] == '#') || (hex.length() == 10 && hex.substr(0, 2) == "0x"))
			argb = static_cast<uint32_t>(std::stoul(hex.substr(hex[0] == '#' ? 1 : 2), nullptr, 16));

		uint8 A = (argb >> 24) & 0xFF;
		uint8 R = (argb >> 16) & 0xFF;
		uint8 G = (argb >> 8) & 0xFF;
		uint8 B = (argb) & 0xFF;

		return FLinearColor(FColor(R, G, B, A));
	}
};