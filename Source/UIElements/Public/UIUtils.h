#pragma once

#include "utils/utils.h"

using namespace material_color_utilities;

struct UIUtils {
public:

	static inline FString HexFromArgb(Argb argb) {
		return FString::Printf(TEXT("0x%08x"), argb);
	}

	static inline Argb ArgbFromHex(const FString& Hex) {
		const FString CleanHex = SanitizeHex(Hex);
		if (CleanHex.Len() == 8)
			return static_cast<Argb>(FParse::HexNumber(*CleanHex));
		return 0xffffffff;
	}

	static inline FString HexFromLinearColor(const FLinearColor& Color) {
		FColor sRGB = Color.ToFColor(true);
		return FString::Printf(TEXT("0x%02x%02x%02x%02x"), sRGB.A, sRGB.R, sRGB.G, sRGB.B);
	}

	static inline FLinearColor LinearColorFromHex(const FString& Hex) {
		const FString CleanHex = SanitizeHex(Hex);
		if (CleanHex.Len() == 8) {
			uint32 ARGB = FParse::HexNumber(*CleanHex);
			uint8 A = (ARGB >> 24) & 0xFF;
			uint8 R = (ARGB >> 16) & 0xFF;
			uint8 G = (ARGB >> 8) & 0xFF;
			uint8 B = (ARGB) & 0xFF;
			return FLinearColor(FColor(R, G, B, A));
		}
		return FLinearColor::White;
	}

private:
	static inline FString SanitizeHex(const FString& Hex) {
		if (Hex.StartsWith(TEXT("#")))
			return Hex.RightChop(1);
		if (Hex.StartsWith(TEXT("0x")))
			return Hex.RightChop(2);
		return Hex;
	}
};
