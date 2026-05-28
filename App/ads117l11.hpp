#pragma once

#include <stdint.h>

namespace ADS117L11 {

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
	CRC = 0xf
};

class Driver {
  private:
};

} // namespace ADS117L11
