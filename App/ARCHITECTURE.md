# Platform-Independent Driver Architecture

This document explains the directory structure and design principles for platform-independent drivers in the ADS117L11 project.

## Directory Structure

```
App/
├── abstractions/              # Platform-independent abstractions
│   ├── result.hpp            # Error handling types
│   ├── spi_concepts.hpp      # C++23 concepts for SPI devices
│   └── spi_interface.hpp     # Abstract SPI interface (virtual base class)
│
├── drivers/                  # Platform-independent driver implementations
│   ├── ads117l11/           # ADS117L11 sigma-delta ADC driver
│   │   ├── ads117l11.hpp    # Driver interface (100% platform-independent)
│   │   └── ads117l11.cpp    # Driver implementation (100% platform-independent)
│   │
│   └── spi/                 # Generic SPI driver wrapper
│       ├── spi.hpp          # SPI wrapper using abstraction layer
│       └── spi.cpp          # SPI wrapper implementation
│
├── platform/                # Platform-specific implementations
│   ├── stm32/              # STM32 HAL-based implementations
│   │   ├── stm32_spi.hpp   # STM32 SPI implementation
│   │   └── stm32_spi.cpp   # STM32 SPI implementation (wraps HAL)
│   │
│   └── stub/               # Stub implementations for testing
│       └── stub_spi.hpp    # Mock SPI for unit testing
│
└── CMakeLists.txt          # App-specific build configuration
```

## Key Design Principles

### 1. **Complete Platform Independence**

The `abstractions/`, `drivers/` directories contain **zero platform-specific code**.

- **No HAL includes** in driver headers
- **No microcontroller-specific types** in interfaces
- **Standard C++ types only** (`uint8_t`, `std::span<>`, etc.)

### 2. **Dependency Injection**

Drivers receive their dependencies (SPI interface) through constructors or function parameters.

**Benefits:**
- Drivers don't know about or depend on platform implementations
- Easy to swap implementations without recompiling drivers
- Easy to test with mock implementations

### 3. **Two-Layer Abstraction**

**Layer 1: Concept-Based (Compile-time)**
- `spi_concepts.hpp` defines what a valid SPI device must support
- Used for template-based SPI implementations
- Zero runtime overhead

**Layer 2: Virtual Interface (Runtime)**
- `spi_interface.hpp` provides virtual interface
- Used for dynamic polymorphism if needed
- Slight runtime overhead but maximum flexibility

### 4. **Error Handling**

- Uses `Result<T>` type for operations that can fail
- No exceptions (embedded environment with `-fno-exceptions`)
- Error codes are standard across all platforms

## Building Independently

### Standard Build (with STM32)

```bash
cd /path/to/ads117l11-v2
cmake --preset Debug
cmake --build --preset Debug
```

This builds:
- ✅ STM32 HAL drivers
- ✅ STM32-specific SPI implementation
- ✅ Platform-independent drivers
- ✅ Complete firmware

### Building App Only (for Testing)

```bash
cd App
cmake -B build -DAPP_PLATFORM=STUB
cmake --build build
```

This builds:
- ✅ Stub implementations
- ✅ Platform-independent drivers
- ❌ No STM32 dependencies
- ✅ Perfect for unit testing

### Build Options

```bash
# Stub implementation (no platform dependencies)
cmake -B build -DAPP_PLATFORM=STUB

# STM32 implementation (requires STM32 HAL)
cmake -B build -DAPP_PLATFORM=STM32

# Abstractions only (no implementation)
cmake -B build -DAPP_PLATFORM=NONE
```

## How to Switch Platforms

To switch from STM32 to another platform (e.g., nRF52):

1. **Create new platform directory:**
   ```bash
   mkdir -p App/platform/nrf52
   ```

2. **Implement SPI interface:**
   ```cpp
   // App/platform/nrf52/nrf52_spi.hpp
   class Nrf52Spi : public abstractions::SpiInterface {
       bool Init() override;
       bool Deinit() override;
       bool SetChipSelect(bool enable) override;
       bool Transfer(std::span<const uint8_t> tx_data, 
                    std::span<uint8_t> rx_data) override;
       uint32_t GetMaxTransferSize() const override;
   };
   ```

3. **Update App/CMakeLists.txt:**
   ```cmake
   elseif(APP_PLATFORM STREQUAL "NRF52")
       add_library(app-drivers-platform STATIC
           platform/nrf52/nrf52_spi.cpp
       )
       # Link with nRF52 SDK instead of STM32 HAL
   endif()
   ```

4. **Update root CMakeLists.txt:**
   ```cmake
   set(APP_PLATFORM "NRF52" CACHE STRING "Platform implementation for App drivers")
   ```

That's it! The ADS117L11 driver and all other platform-independent code require **zero changes**.

## Using the Drivers

### With STM32 (Runtime Polymorphism)

```cpp
#include "App/platform/stm32/stm32_spi.hpp"
#include "App/drivers/ads117l11/ads117l11.hpp"

int main() {
    // Create platform-specific SPI implementation
    extern SPI_HandleTypeDef hspi1;
    platform::stm32::Stm32Spi spi(&hspi1);
    
    // Create driver (doesn't know it's using STM32)
    drivers::ADS117L11Driver driver(&spi);
    
    // Use driver (100% platform-independent code path)
    driver.Init();
    // ... driver operations
}
```

### With Stub (for Testing)

```cpp
#include "App/platform/stub/stub_spi.hpp"
#include "App/drivers/ads117l11/ads117l11.hpp"

void test_driver() {
    // Create stub SPI for testing
    platform::stub::StubSpi spi;
    
    // Create driver with stub
    drivers::ADS117L11Driver driver(&spi);
    
    // Test the driver
    driver.Init();
    // ... verify behavior
    
    // Inspect what happened
    const auto& tx_data = spi.GetLastTransferTx();
    size_t tx_size = spi.GetLastTransferTxSize();
}
```

## File: abstractions/result.hpp

Provides error handling without exceptions.

```cpp
// Success case
auto result = operation();
if (result.IsOk()) {
    int value = result.GetValue();
}

// Error case
if (!result.IsOk()) {
    ErrorCode err = result.GetError();
}
```

## File: abstractions/spi_concepts.hpp

C++23 concepts for compile-time validation of SPI devices.

```cpp
template <typename SpiImpl>
requires abstractions::SpiDevice<SpiImpl>
class Driver {
    // Guaranteed that SpiImpl has all required methods
    // Compile error if it doesn't
};
```

## File: abstractions/spi_interface.hpp

Abstract base class for runtime polymorphism.

```cpp
class Driver {
    SpiInterface* spi_;  // Can be any implementation
    // Runtime virtual dispatch
};
```

## File: drivers/spi/spi.hpp

Wrapper around the abstract SPI interface, providing convenience methods.

## Platform-Specific Files

### stm32_spi.hpp / stm32_spi.cpp

Wraps STM32 HAL SPI functions into the `SpiInterface` contract.

- Includes `stm32h7xx_hal.h` (only platform-specific include)
- Converts `std::span` to HAL function calls
- Handles timing and error checking

### stub_spi.hpp

Mock SPI implementation for testing without hardware.

- Records all transfers for inspection
- Returns configurable dummy data
- No hardware dependencies

## Adding New Drivers

When adding a new driver (e.g., temperature sensor):

1. **Create driver in abstractions:**
   ```bash
   mkdir -p App/drivers/temperature
   ```

2. **Depend on abstractions, not platform:**
   ```cpp
   // temperature.hpp
   #include "../abstractions/spi_interface.hpp"
   
   class TemperatureDriver {
       explicit TemperatureDriver(abstractions::SpiInterface* spi);
       // 100% platform-independent
   };
   ```

3. **Never include platform-specific headers** - driver stays portable

## Testing

The stub implementation makes it easy to test drivers without hardware:

```cpp
#include "gtest/gtest.h"
#include "App/platform/stub/stub_spi.hpp"
#include "App/drivers/ads117l11/ads117l11.hpp"

TEST(ADS117L11, InitSequence) {
    platform::stub::StubSpi spi;
    drivers::ADS117L11Driver driver(&spi);
    
    EXPECT_TRUE(driver.Init());
    EXPECT_EQ(spi.GetLastTransferTxSize(), 2);  // Verify init sequence
}
```

## Summary

| Aspect | Benefit |
|--------|---------|
| **Platform Independence** | Switch MCUs by implementing one file |
| **Testability** | Use stub implementation, no hardware needed |
| **Modularity** | Drivers are decoupled from platform |
| **Maintainability** | Platform code isolated, easy to update |
| **Reusability** | Drivers work on any platform with SPI |
| **Type Safety** | C++23 concepts with zero overhead |
