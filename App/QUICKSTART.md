## Quick Start: Using the Platform-Independent Architecture

### 1. Understand the Three Layers

```
Your Application Code
        ↓
drivers/ads117l11/     (Platform-independent driver)
        ↓
abstractions/spi_*     (Interface definitions)
        ↓
platform/stm32/ OR platform/stub/ OR your own
```

### 2. Implementing a New SPI-Based Driver

Let's say you want to add a driver for an SPI temperature sensor.

**Step 1: Create the driver header** (`App/drivers/temperature/temperature.hpp`)

```cpp
#pragma once

#include <cstdint>
#include "../../abstractions/spi_interface.hpp"

namespace drivers {

class TemperatureDriver {
   public:
    explicit TemperatureDriver(abstractions::SpiInterface* spi) 
        : spi_(spi) {}
    
    bool Init() {
        // Implementation doesn't know or care about platform
        // Just uses SPI interface
        return spi_->SetChipSelect(true) && 
               spi_->Transfer(init_cmd, {});
    }
    
    bool ReadTemperature(float& temp_out) {
        uint8_t rx_buffer[2] = {};
        if (!spi_->Transfer({}, rx_buffer)) {
            return false;
        }
        // Process raw data to temperature
        temp_out = (rx_buffer[0] << 8 | rx_buffer[1]) * 0.0625f;
        return true;
    }
   
   private:
    abstractions::SpiInterface* spi_;
    static constexpr uint8_t init_cmd[] = {0xAA};
};

}  // namespace drivers
```

**Step 2: Use it with any platform**

With STM32:
```cpp
#include "App/platform/stm32/stm32_spi.hpp"
#include "App/drivers/temperature/temperature.hpp"

int main() {
    extern SPI_HandleTypeDef hspi1;
    platform::stm32::Stm32Spi spi(&hspi1);
    drivers::TemperatureDriver temp_driver(&spi);
    
    temp_driver.Init();
    float temp;
    temp_driver.ReadTemperature(temp);
}
```

With Stub (for testing):
```cpp
#include "App/platform/stub/stub_spi.hpp"
#include "App/drivers/temperature/temperature.hpp"

void test_temperature() {
    platform::stub::StubSpi spi;
    spi.SetStubResponseByte(0x50);  // Mock response
    
    drivers::TemperatureDriver temp_driver(&spi);
    float temp;
    temp_driver.ReadTemperature(temp);
}
```

**Same driver, different platforms!**

### 3. Switching Platforms

Current: STM32H753
Future: nRF52840

**What changes?**
- Only `App/platform/nrf52/nrf52_spi.cpp` (implement SpiInterface)
- Update `APP_PLATFORM` in CMakeLists.txt
- Everything else stays the same

**What stays the same?**
- `drivers/ads117l11/`
- `drivers/temperature/` (if you add it)
- All your application code

### 4. Testing Without Hardware

```cpp
#include "App/platform/stub/stub_spi.hpp"

void test_ads117l11_init() {
    // Create stub SPI (no hardware needed!)
    platform::stub::StubSpi spi;
    
    // Create driver with stub
    ADS117L11Driver driver(&spi);
    
    // Test initialization sequence
    ASSERT_TRUE(driver.Init());
    
    // Inspect what was transmitted
    const auto& tx_buffer = spi.GetLastTransferTx();
    EXPECT_EQ(tx_buffer[0], 0x40);  // Expected register read command
}
```

### 5. CMake Configuration

**To build with platform-independent code only:**
```bash
cd App
cmake -B build -DAPP_PLATFORM=STUB
cmake --build build
```

**To build full STM32 project:**
```bash
cd ..
cmake --preset Debug
cmake --build build/Debug
```

### 6. Directory Quick Reference

| Path | Purpose | Platform-Dependent? |
|------|---------|-------------------|
| `abstractions/` | Interface definitions | No |
| `drivers/` | Driver implementations | No |
| `platform/stm32/` | STM32 HAL wrapper | Yes |
| `platform/stub/` | Mock for testing | No |
| `platform/nrf52/` | (Future) nRF52 wrapper | Yes (when added) |

### 7. Key Files to Understand

1. **abstractions/spi_interface.hpp** - The contract that all SPI implementations must follow
2. **platform/stub/stub_spi.hpp** - How to implement the SPI contract (good reference)
3. **platform/stm32/stm32_spi.cpp** - Real HAL integration (platform-specific)
4. **drivers/ads117l11/ads117l11.hpp** - Example of platform-independent driver

### 8. Adding a New Platform

Example: Adding POSIX SPI simulator

```bash
mkdir -p App/platform/posix
```

Create `App/platform/posix/posix_spi.hpp`:

```cpp
#include "../../abstractions/spi_interface.hpp"
#include <unistd.h>

namespace platform::posix {

class PosixSpi : public abstractions::SpiInterface {
    bool Init() override { /* call linux SPI ioctl */ }
    bool Transfer(std::span<const uint8_t> tx, 
                 std::span<uint8_t> rx) override { 
        /* call ::write/::read on /dev/spidevX.Y */ 
    }
    // ... other methods
};

}
```

Update `App/CMakeLists.txt`:
```cmake
elseif(APP_PLATFORM STREQUAL "POSIX")
    add_library(app-drivers-platform STATIC
        platform/posix/posix_spi.cpp
    )
endif()
```

Now you can test your drivers on a Linux development machine!

### 9. Error Handling Example

Using the `Result<T>` type for safe error handling:

```cpp
// In a driver
abstractions::Result<uint16_t> ReadRegister(uint8_t addr) {
    uint8_t cmd[] = {addr};
    uint8_t response[2];
    
    if (!spi_->Transfer(cmd, response)) {
        return abstractions::Result<uint16_t>::Error(
            abstractions::ErrorCode::SPI_ERROR);
    }
    
    uint16_t value = (response[0] << 8) | response[1];
    return abstractions::Result<uint16_t>::Success(value);
}

// In application code
auto result = driver.ReadRegister(0x00);
if (result.IsOk()) {
    uint16_t reg_value = result.GetValue();
    // Process value
} else {
    // Handle error
    if (result.GetError() == abstractions::ErrorCode::SPI_ERROR) {
        // Retry or fail
    }
}
```

---

**That's it!** You now have a fully platform-independent driver architecture. 

Want to support 10 different MCUs? Add 10 platform implementations, keep all drivers unchanged.

Want to test on your laptop? Use the stub implementation, no embedded hardware needed.
