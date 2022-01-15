#pragma once

#include <cstdint>

namespace wasp::utility {
	inline uint16_t byteSwap16(uint16_t i);
	inline uint32_t byteSwap32(uint32_t i);
	inline uint64_t byteSwap64(uint64_t i);
}