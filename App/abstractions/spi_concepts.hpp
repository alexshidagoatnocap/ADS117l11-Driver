#pragma once

#include "abstractions/spi_interface.hpp"
#include <concepts>

namespace DAL {

// TODO: Validate methods in derived class as well
template <typename T>
concept SpiImpl = std::derived_from<T, SpiInterface>;

} // namespace DAL
