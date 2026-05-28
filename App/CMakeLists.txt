cmake_minimum_required(VERSION 3.22)

# This CMakeLists.txt allows the App/ directory to be built independently
# It can be used for:
# 1. Standalone testing of drivers
# 2. Unit testing with stub implementations
# 3. Building only the platform-independent code

project(app-drivers)

# Enable C++23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Platform selection for independent builds
# Options:
#   - NONE: Only abstractions and platform-independent drivers (no SPI backend)
#   - STM32: Use STM32 HAL SPI implementation
#   - STUB: Use stub implementation for testing
if(NOT DEFINED APP_PLATFORM)
    set(APP_PLATFORM "STUB" CACHE STRING "Platform implementation: NONE, STM32, or STUB")
endif()

message(STATUS "App Platform: ${APP_PLATFORM}")

# Create a library with platform-independent code
add_library(app-drivers-core INTERFACE)

# Add include directories
target_include_directories(app-drivers-core INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/
)

# Platform-specific sources
if(APP_PLATFORM STREQUAL "STM32")
    # When building with STM32 platform
    # Requires: STM32 HAL headers (from cmake/stm32cubemx)
    add_library(app-drivers-platform STATIC
        platform/stm32/stm32_spi.cpp
    )
    
    target_link_libraries(app-drivers-platform PUBLIC app-drivers-core)
    target_include_directories(app-drivers-platform PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
    
    # Link against STM32CubeMX libraries (if available)
    if(TARGET stm32cubemx)
        target_link_libraries(app-drivers-platform PUBLIC stm32cubemx)
    endif()

elseif(APP_PLATFORM STREQUAL "STUB")
    # When building with stub platform for testing
    add_library(app-drivers-platform INTERFACE)
    target_link_libraries(app-drivers-platform INTERFACE app-drivers-core)
    target_include_directories(app-drivers-platform INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

else()
    # NONE - only abstractions, no platform implementation
    add_library(app-drivers-platform INTERFACE)
    target_link_libraries(app-drivers-platform INTERFACE app-drivers-core)
    target_include_directories(app-drivers-platform INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
endif()

# Export for use by parent projects
set(APP_DRIVERS_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE PATH "App drivers include directory" FORCE)
set(APP_DRIVERS_PLATFORM_LIB app-drivers-platform CACHE INTERNAL "App drivers platform library" FORCE)
set(APP_DRIVERS_CORE_LIB app-drivers-core CACHE INTERNAL "App drivers core library" FORCE)
