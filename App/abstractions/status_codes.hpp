#pragma once

#include <cstdint>

namespace DAL {

enum class Status : uint8_t {
	ABSTRACT_OK,
	ABSTRACT_INIT_FAIL,
	ABSTRACT_NOT_INIT,
	ABSTRACT_ERROR
};

enum class PinState : bool { LOW = false, HIGH = true };

} // namespace DAL
