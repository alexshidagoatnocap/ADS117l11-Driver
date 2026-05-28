# Dependency Flow Diagram

## How Platform Independence Works

```
YOUR APPLICATION CODE
        ↓
        ↓ uses
        ↓
┌─────────────────────────────────┐
│    drivers/ads117l11/           │  ← 100% Platform-Independent
│  (ADS117L11Driver)              │     No HAL includes
└─────────────────────────────────┘     No platform-specific types
        ↓
        ↓ depends on
        ↓
┌─────────────────────────────────┐
│  abstractions/spi_interface.hpp │  ← Interface Contract
│  (abstract SpiInterface)        │     Defines what SPI must do
└─────────────────────────────────┘     NO implementation
        ↑
        ↑ implements (you choose one)
        ↑
    ┌───┴───────────────────────────┐
    │                               │
┌───────────────────┐      ┌────────────────────┐
│ platform/stm32/   │      │ platform/stub/     │
│ Stm32Spi          │      │ StubSpi            │
│ (real hardware)   │      │ (testing/mocking)  │
└───────────────────┘      └────────────────────┘
    ↓                           ↓
    ↓ links                     ↓ links
    ↓                           ↓
STM32 HAL                   Nothing!
```

## Code Dependency Example

### Platform-Independent Layer
```cpp
// drivers/ads117l11/ads117l11.hpp
#include "../../abstractions/spi_interface.hpp"  // ← ONLY abstraction!

class ADS117L11Driver {
    // Constructor takes abstract interface
    ADS117L11Driver(abstractions::SpiInterface* spi) : spi_(spi) {}
    
    bool Init() {
        // Uses ONLY the abstract interface
        spi_->SetChipSelect(true);
        spi_->Transfer(...);
        // ↑ Doesn't care WHAT implements SpiInterface
    }
};
```

### Platform-Specific Layer
```cpp
// platform/stm32/stm32_spi.cpp
#include "stm32h7xx_hal.h"  // ← Only here!
#include "stm32_spi.hpp"

bool Stm32Spi::Transfer(...) {
    // Real STM32 HAL calls
    HAL_SPI_TransmitReceive(...);
}
```

### Application Code
```cpp
// Your code - works with ANY platform!
int main() {
    // On STM32:
    platform::stm32::Stm32Spi spi(&hspi1);
    ADS117L11Driver driver(&spi);  // Uses STM32
    
    // On nRF52: Just change this one line!
    // platform::nrf52::Nrf52Spi spi(&nrf_spi);
    // ADS117L11Driver driver(&spi);  // Uses nRF52
    
    // Rest of code is IDENTICAL
}
```

## Build System Dependency

```
root CMakeLists.txt (project build)
    ↓
    ├─→ cmake/stm32cubemx/          (STM32 HAL sources)
    │
    ├─→ App/CMakeLists.txt          (App drivers)
    │       ↓
    │       ├─→ abstractions/       (headers only, no compilation)
    │       ├─→ drivers/            (compile platform-independent code)
    │       └─→ platform/stm32/     (compile STM32 implementation)
    │               ↓
    │               └─→ includes stm32h7xx_hal.h
    │
    └─→ final executable: ads117l11-v2.elf

INDEPENDENT APP BUILD:
App/CMakeLists.txt (standalone)
    ↓
    ├─→ abstractions/       (headers only)
    ├─→ drivers/            (compile drivers)
    └─→ platform/stub/      (compile stub - NO HAL!)
```

## Key Insight: The Contract

```
abstractions/spi_interface.hpp is THE CONTRACT:

    ┌─────────────────────────────────────┐
    │  interface SpiInterface {           │
    │    bool Init()                      │
    │    bool Transfer(tx, rx)            │
    │    bool SetChipSelect(enabled)      │
    │    uint32_t GetMaxTransferSize()    │
    └─────────────────────────────────────┘
         ↑                  ↑
         │                  │
    "I promise to              "Any code using this
     implement all these       doesn't care how"
     methods"
```

Any class implementing this interface can be used:
- `Stm32Spi` ✓ (implements all methods → HAL)
- `StubSpi` ✓ (implements all methods → mock)
- `Nrf52Spi` ✓ (implements all methods → nRF52 SDK)
- `FakeSpi` ✓ (implements all methods → test fake)

The ADS117L11 driver only knows about the interface, not the implementations.

## Two Patterns Mixed

This project uses BOTH abstraction patterns:

### Pattern 1: Virtual Interface (Runtime Polymorphism)
```cpp
class SpiInterface {  // abstract base
    virtual bool Transfer(...) = 0;
};

// Works at runtime:
SpiInterface* spi = /* could be StubSpi, Stm32Spi, etc. */
driver.Init(spi);  // Decision made at runtime
```

### Pattern 2: Concepts (Compile-Time)
```cpp
template <SpiDevice T>  // T must satisfy concept
class Driver {
    // Compiler checks T has all required methods
};

// Works at compile-time:
Driver<StubSpi> test_driver;    // Compile error if StubSpi missing methods
Driver<Stm32Spi> real_driver;   // Type-safe, zero overhead
```

### When to Use Which

**Use Virtual Interface** when:
- You need runtime selection
- Testing with different implementations
- Plugin architecture

**Use Concepts** when:
- You want zero overhead (embedded)
- Template code anyway
- All implementations known at compile time

**Use Both** (like this project):
- Flexibility + performance
- Easy testing + efficient production

## Adding New Drivers

```
Current State:
  abstractions/      (SPI contract)
      ↓
  drivers/ads117l11/ (uses SPI)
      ↓
  platform/stm32/    (implements SPI)

Add Temperature Driver:
  abstractions/      (SPI contract)
      ↓
  drivers/temp/      (uses SPI) ← NEW
  drivers/ads117l11/ (uses SPI)
      ↓
  platform/stm32/    (implements SPI)

The temperature driver doesn't know about STM32!
Same contract, new driver, same platform layer.
```

## Testing Without Hardware

```
Test Environment:
  abstractions/      (SPI contract)
      ↓
  drivers/ads117l11/ (uses SPI)
      ↓
  platform/stub/     (mock implementation)
          ↓
      No hardware!   Just test the logic

Production:
  abstractions/      (SPI contract)
      ↓
  drivers/ads117l11/ (uses SPI)  ← SAME CODE!
      ↓
  platform/stm32/    (real implementation)
          ↓
      Real hardware
```

## Summary

1. **Abstractions** = Contracts (no implementation)
2. **Drivers** = Use abstractions (no platform knowledge)
3. **Platform** = Implement abstractions (only real code here)
4. **Your App** = Uses drivers (oblivious to platforms)

Result: Change platforms by replacing ONE directory.
