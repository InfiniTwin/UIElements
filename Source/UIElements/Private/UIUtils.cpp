// Fill out your copyright notice in the Description page of Project Settings.


#include "UIUtils.h"
#include <sstream>
#include <iomanip>

using namespace material_color_utilities;

std::string UIUtils::HexFromArgb(Argb argb) {
	std::stringstream ss;
	ss << "0x" << std::setw(8) << std::setfill('0') << std::hex << argb;
	return ss.str();
}

Argb UIUtils::ArgbFromHex(const std::string& hex) {
    if ((hex.length() == 9 && hex[0] == '#') || (hex.length() == 10 && hex.substr(0, 2) == "0x")) {
        return static_cast<Argb>(std::stoul(hex.substr(hex[0] == '#' ? 1 : 2), nullptr, 16));
    }
    return 0xffffffff;
}
