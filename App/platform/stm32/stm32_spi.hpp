#pragma once

#include <cstdint>
#include <span>
#include "stm32h7xx_hal.h"

#include "../../abstractions/spi_interface.hpp"

namespace platform::stm32 {

/// STM32-specific SPI implementation
/// Wraps the STM32 HAL SPI interface
class Stm32Spi : public abstractions::SpiInterface {
   public:
    /// Initialize with an STM32 SPI handle
    /// @param hspi Pointer to STM32 SPI_HandleTypeDef (must be pre-initialized by user)
    explicit Stm32Spi(SPI_HandleTypeDef* hspi);

    bool Init() override;
    bool Deinit() override;
    bool SetChipSelect(bool enable) override;
    bool Transfer(std::span<const uint8_t> tx_data, std::span<uint8_t> rx_data) override;
    uint32_t GetMaxTransferSize() const override;

   private:
    SPI_HandleTypeDef* hspi_;
    bool initialized_ = false;
};

}  // namespace platform::stm32
