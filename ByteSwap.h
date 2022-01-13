#pragma once

#include <cstdint>
#include <intrin.h>

namespace wasp::utility {
	inline uint16_t byteSwap16(uint16_t i) {
		return _byteswap_ushort(i);
	}

	inline uint32_t byteSwap32(uint32_t i) {
		return _byteswap_ulong(i);
	}

	inline uint64_t byteSwap64(uint64_t i) {
		return _byteswap_uint64(i);
	}
}