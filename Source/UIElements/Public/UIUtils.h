// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <string>
#include "utils/utils.h"

struct UIUtils {
public:
	static std::string HexFromArgb(material_color_utilities::Argb argb);
	static:: material_color_utilities::Argb ArgbFromHex(const std::string& hex);
};