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
};