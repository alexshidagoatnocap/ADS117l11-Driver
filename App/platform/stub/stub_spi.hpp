#pragma once

#include <cstdint>
#include <span>
#include <array>
#include "../abstractions/spi_interface.hpp"

namespace platform::stub {

/// Stub SPI implementation for unit testing
/// Records all transfers and can be configured to return specific data
class StubSpi : public abstractions::SpiInterface {
   public:
    static constexpr uint32_t MAX_TRANSFER_SIZE = 256;

    bool Init() override {
        initialized_ = true;
        return true;
    }

    bool Deinit() override {
        initialized_ = false;
        return true;
    }

    bool SetChipSelect(bool enable) override {
        cs_enabled_ = enable;
        return true;
    }

    bool Transfer(std::span<const uint8_t> tx_data, std::span<uint8_t> rx_data) override {
        if (!initialized_) {
            return false;
        }

        // Record this transfer for testing/inspection
        last_transfer_tx_size_ = tx_data.size();
        last_transfer_rx_size_ = rx_data.size();

        // Copy tx data to buffer if provided
        if (!tx_data.empty() && tx_data.size() <= MAX_TRANSFER_SIZE) {
            for (size_t i = 0; i < tx_data.size(); ++i) {
                last_transfer_tx_[i] = tx_data[i];
            }
        }

        // Fill rx data with stub response (0xAA by default, can be overridden)
        if (!rx_data.empty()) {
            for (size_t i = 0; i < rx_data.size(); ++i) {
                rx_data[i] = stub_response_byte_;
            }
        }

        return true;
    }

    uint32_t GetMaxTransferSize() const override {
        return MAX_TRANSFER_SIZE;
    }

    // Test helper methods
    void SetStubResponseByte(uint8_t byte) {
        stub_response_byte_ = byte;
    }

    const std::array<uint8_t, MAX_TRANSFER_SIZE>& GetLastTransferTx() const {
        return last_transfer_tx_;
    }

    size_t GetLastTransferTxSize() const {
        return last_transfer_tx_size_;
    }

    size_t GetLastTransferRxSize() const {
        return last_transfer_rx_size_;
    }

    bool IsInitialized() const {
        return initialized_;
    }

    bool IsChipSelectEnabled() const {
        return cs_enabled_;
    }

   private:
    bool initialized_ = false;
    bool cs_enabled_ = false;
    uint8_t stub_response_byte_ = 0xAA;
    std::array<uint8_t, MAX_TRANSFER_SIZE> last_transfer_tx_{};
    size_t last_transfer_tx_size_ = 0;
    size_t last_transfer_rx_size_ = 0;
};

}  // namespace platform::stub
