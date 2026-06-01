#pragma once

#include "abstractions/status_codes.hpp"
#include <cstdint>
#include <span>
#include <utility>

namespace DAL {

class SpiInterface {
  public:
	~SpiInterface() = default;

	template <typename... Args>
	DAL::Status init(this auto &&self, Args &&...args) {
		return self.initImpl(std::forward<Args>(args)...);
	}

	DAL::Status deinit(this auto &&self) { return self.deinitImpl(); }

	DAL::Status csHigh(this auto &&self) { return self.csHighImpl(); }

	DAL::Status csLow(this auto &&self) { return self.csLowImpl(); }

	DAL::Status transmit(this auto &&self, std::span<const uint8_t> tx_data) {
		return self.transmitImpl(tx_data);
	}

	DAL::Status receive(this auto &&self, std::span<uint8_t> rx_data) {
		return self.receiveImpl(rx_data);
	}

	DAL::Status transmitReceive(this auto &&self,
								std::span<const uint8_t> tx_data,
								std::span<uint8_t> rx_data) {
		return self.transmitReceiveImpl(tx_data, rx_data);
	}

  protected:
	DAL::Status initImpl();
	DAL::Status deinitImpl();
	DAL::Status csHighImpl();
	DAL::Status csLowImpl();
	DAL::Status transmitImpl();
	DAL::Status receiveImpl();
	DAL::Status transmitReceiveImpl();
};

} // namespace DAL
