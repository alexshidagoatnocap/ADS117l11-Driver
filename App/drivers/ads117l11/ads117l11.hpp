#pragma once

#include "abstractions/spi_concepts.hpp"

namespace drivers {

template <DAL::SpiImpl Spi> class ADS117L11 {
	Spi m_spiObj{};

  public:
	explicit ADS117L11(Spi spiObj) = default;
};
} // namespace drivers
