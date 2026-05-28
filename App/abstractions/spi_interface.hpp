#pragma once

#include <cstdint>
#include <span>

namespace abstractions {

class SpiInterface {
  public:
	~SpiInterface() = default;

	bool Init();

	bool Deinit();

	/// Enable/disable chip select
	bool SetChipSelect(bool enable);

	/// Perform SPI transaction
	/// @param tx_data Data to transmit (can be empty for rx-only)
	/// @param rx_data Buffer to receive data into (can be empty for tx-only)
	/// @return true on success, false on error
	bool Transfer(std::span<const uint8_t> tx_data, std::span<uint8_t> rx_data);
};

} // namespace abstractions
