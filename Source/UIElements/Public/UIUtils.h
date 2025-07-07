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

private:
	static inline FString SanitizeHex(const FString& Hex) {
		if (Hex.StartsWith(TEXT("#")))
			return Hex.RightChop(1);
		if (Hex.StartsWith(TEXT("0x")))
			return Hex.RightChop(2);
		return Hex;
	}
};
