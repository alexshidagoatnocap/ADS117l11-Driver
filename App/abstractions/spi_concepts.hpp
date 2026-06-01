#pragma once

#include "abstractions/spi_interface.hpp"
#include <concepts>
#include <cstdint>
#include <span>

namespace DAL {

// TODO: Validate methods in derived class as well
template <typename T>
concept SpiImpl = std::derived_from<T, SpiInterface>;

} // namespace DAL
