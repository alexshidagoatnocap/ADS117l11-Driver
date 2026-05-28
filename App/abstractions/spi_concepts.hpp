#pragma once

#include <cstdint>
#include <concepts>
#include <span>

namespace abstractions {

/// Concept for SPI device implementations
/// Any type satisfying this concept can be used as an SPI driver
template <typename T>
concept SpiDevice = requires(T& device, std::span<const uint8_t> tx_data, std::span<uint8_t> rx_data) {
    /// Initialize the SPI device
    { device.Init() } -> std::convertible_to<bool>;

    /// Deinitialize the SPI device
    { device.Deinit() } -> std::convertible_to<bool>;

    /// Enable/disable chip select
    { device.SetChipSelect(true) } -> std::convertible_to<bool>;

    /// Perform SPI transaction (send tx_data, receive into rx_data)
    /// If only transmitting, rx_data can be empty
    /// If only receiving, tx_data can be empty
    { device.Transfer(tx_data, rx_data) } -> std::convertible_to<bool>;

    /// Get maximum transfer size supported by this SPI device
    { device.GetMaxTransferSize() } -> std::convertible_to<uint32_t>;
};

}  // namespace abstractions
