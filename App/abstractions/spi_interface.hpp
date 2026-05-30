#pragma once

#include "abstractions/status_codes.hpp"
#include <cstdint>
#include <span>

namespace abstractions {

class SpiInterface {
  public:
	~SpiInterface() = default;

	abstractions::Status init(this auto &&self) { return self.initImpl(); }

	abstractions::Status deinit(this auto &&self) { return self.deinitImpl(); }

	abstractions::Status setChipSelect(this auto &&self,
									   abstractions::PinState state) {
		return self.setChipSelectImpl(state);
	}

	abstractions::Status transmitReceive(this auto &&self,
										 std::span<const uint8_t> tx_data,
										 std::span<uint8_t> rx_data) {
		return self.transmitReceiveImpl(tx_data, rx_data);
	}
};

} // namespace abstractions
