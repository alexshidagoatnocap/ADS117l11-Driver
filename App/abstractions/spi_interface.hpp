#pragma once

#include <cstdint>
#include <span>
#include "result.hpp"

namespace abstractions {

/// Abstract interface for SPI device
/// Provides a virtual interface for runtime polymorphism
/// Can be used instead of concepts if runtime flexibility is needed
class SpiInterface {
   public:
    virtual ~SpiInterface() = default;

    /// Initialize the SPI device
    virtual bool Init() = 0;

    /// Deinitialize the SPI device
    virtual bool Deinit() = 0;

    /// Enable/disable chip select
    virtual bool SetChipSelect(bool enable) = 0;

    /// Perform SPI transaction
    /// @param tx_data Data to transmit (can be empty for rx-only)
    /// @param rx_data Buffer to receive data into (can be empty for tx-only)
    /// @return true on success, false on error
    virtual bool Transfer(std::span<const uint8_t> tx_data, std::span<uint8_t> rx_data) = 0;

    /// Get maximum transfer size supported
    virtual uint32_t GetMaxTransferSize() const = 0;
};

}  // namespace abstractions
