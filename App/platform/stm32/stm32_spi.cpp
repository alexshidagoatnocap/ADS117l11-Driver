#include "stm32_spi.hpp"
#include "stm32h7xx_hal.h"

namespace platform::stm32 {

Stm32Spi::Stm32Spi(SPI_HandleTypeDef* hspi) : hspi_(hspi) {}

bool Stm32Spi::Init() {
    if (!hspi_) {
        return false;
    }
    // SPI is assumed to be initialized by STM32CubeMX before this is called
    initialized_ = true;
    return true;
}

bool Stm32Spi::Deinit() {
    if (!hspi_) {
        return false;
    }
    // HAL deinitialization can be done here if needed
    initialized_ = false;
    return true;
}

bool Stm32Spi::SetChipSelect(bool enable) {
    // Chip select handling depends on your specific setup
    // This is typically handled by GPIO, not the SPI peripheral itself
    // You may need to add GPIO configuration here
    // For now, this is a placeholder
    return true;
}

bool Stm32Spi::Transfer(std::span<const uint8_t> tx_data, std::span<uint8_t> rx_data) {
    if (!hspi_ || !initialized_) {
        return false;
    }

    if (tx_data.empty() && rx_data.empty()) {
        return false;  // Nothing to transfer
    }

    // Handle case where we have both tx and rx data
    if (!tx_data.empty() && !rx_data.empty()) {
        if (tx_data.size() != rx_data.size()) {
            return false;  // Sizes must match for full-duplex
        }

        HAL_StatusTypeDef status = HAL_SPI_TransmitReceive(
            hspi_, (uint8_t*)tx_data.data(), rx_data.data(),
            (uint16_t)tx_data.size(), HAL_MAX_DELAY);
        return status == HAL_OK;
    }

    // Handle transmit-only
    if (!tx_data.empty()) {
        HAL_StatusTypeDef status =
            HAL_SPI_Transmit(hspi_, (uint8_t*)tx_data.data(), (uint16_t)tx_data.size(), HAL_MAX_DELAY);
        return status == HAL_OK;
    }

    // Handle receive-only
    if (!rx_data.empty()) {
        // For receive-only, we transmit zeros
        // This is a limitation - we'd need a buffer for proper dummy transmit
        // For now, this is a simplified version
        HAL_StatusTypeDef status =
            HAL_SPI_Receive(hspi_, rx_data.data(), (uint16_t)rx_data.size(), HAL_MAX_DELAY);
        return status == HAL_OK;
    }

    return false;
}

uint32_t Stm32Spi::GetMaxTransferSize() const {
    // STM32H7 SPI can handle up to 65535 bytes in a single transaction
    return 65535;
}

}  // namespace platform::stm32
