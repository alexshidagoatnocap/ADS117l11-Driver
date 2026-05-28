#pragma once

#include <cstdint>
#include <span>
#include "../abstractions/spi_interface.hpp"
#include "../abstractions/spi_concepts.hpp"

namespace drivers::spi {

/// Generic SPI driver that can work with any platform-specific SPI implementation
/// Provides a common interface for protocol drivers like ADS117L11
class Spi {
   public:
    explicit Spi(abstractions::SpiInterface* hw_interface) : hw_interface_(hw_interface) {}

    /// Initialize SPI interface
    bool Init() {
        if (!hw_interface_) {
            return false;
        }
        return hw_interface_->Init();
    }

    /// Deinitialize SPI interface
    bool Deinit() {
        if (!hw_interface_) {
            return false;
        }
        return hw_interface_->Deinit();
    }

    /// Set chip select state
    bool SetChipSelect(bool enable) {
        if (!hw_interface_) {
            return false;
        }
        return hw_interface_->SetChipSelect(enable);
    }

    /// Transfer data over SPI
    bool Transfer(std::span<const uint8_t> tx_data, std::span<uint8_t> rx_data) {
        if (!hw_interface_) {
            return false;
        }
        return hw_interface_->Transfer(tx_data, rx_data);
    }

    /// Transmit only (no receive)
    bool Transmit(std::span<const uint8_t> data) {
        return Transfer(data, std::span<uint8_t>());
    }

    /// Receive only (no transmit, transmit zeros)
    bool Receive(std::span<uint8_t> data) {
        return Transfer(std::span<const uint8_t>(), data);
    }

    /// Get maximum transfer size
    uint32_t GetMaxTransferSize() const {
        if (!hw_interface_) {
            return 0;
        }
        return hw_interface_->GetMaxTransferSize();
    }

   private:
    abstractions::SpiInterface* hw_interface_;
};

}  // namespace drivers::spi
