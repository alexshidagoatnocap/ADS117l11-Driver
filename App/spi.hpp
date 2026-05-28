#pragma once

#include <concepts>

template <typename T, typename Base>
concept IsBaseSpi = std::same_as<std::decay_t<T>, Base>;

class SpiInterface {

  public:
	SpiInterface();
};
