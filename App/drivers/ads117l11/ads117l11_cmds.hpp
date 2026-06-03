#pragma once

#include <stdint.h>
#include <utility>

namespace drivers::ADS117L11 {

enum class Command : uint8_t { READ_REG = 0x40, WRITE_REG = 0x80 };

enum class Register : uint8_t {
	DEV_ID = 0x0,
	REV_ID = 0x1,
	STATUS = 0x2,
	CONTROL = 0x3,
	MUX = 0x4,
	CONFIG1 = 0x5,
	CONFIG2 = 0x6,
	CONFIG3 = 0x7,
	CONFIG4 = 0x8,
	OFFSET2 = 0x9,
	OFFSET1 = 0xa,
	OFFSET0 = 0xb,
	GAIN1 = 0xc,
	GAIN0 = 0xd,
	CRC_REG = 0xf
};

inline uint8_t operator+(drivers::ADS117L11::Command cmd,
						 drivers::ADS117L11::Register reg) {
	return std::to_underlying(cmd) + std::to_underlying(reg);
}

} // namespace drivers::ADS117L11
