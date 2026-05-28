#pragma once

#include <cstdint>
#include <optional>

namespace abstractions {

/// Error codes for driver operations
enum class ErrorCode : uint8_t {
    OK = 0,
    SPI_ERROR = 1,
    TIMEOUT = 2,
    INVALID_ARGUMENT = 3,
    DEVICE_NOT_READY = 4,
    COMMUNICATION_ERROR = 5,
};

/// Result type for operations that can fail
template <typename T>
class Result {
   public:
    /// Construct success result
    static constexpr Result Success(T value) {
        Result r;
        r.value_ = value;
        r.error_ = ErrorCode::OK;
        return r;
    }

    /// Construct error result
    static constexpr Result Error(ErrorCode error) {
        Result r;
        r.error_ = error;
        return r;
    }

    /// Check if operation succeeded
    [[nodiscard]] constexpr bool IsOk() const { return error_ == ErrorCode::OK; }

    /// Get error code
    [[nodiscard]] constexpr ErrorCode GetError() const { return error_; }

    /// Get value (only valid if IsOk() returns true)
    [[nodiscard]] constexpr T GetValue() const { return value_; }

   private:
    T value_{};
    ErrorCode error_ = ErrorCode::OK;
};

/// Specialization for void results
template <>
class Result<void> {
   public:
    /// Construct success result
    static constexpr Result Success() {
        return Result();
    }

    /// Construct error result
    static constexpr Result Error(ErrorCode error) {
        Result r;
        r.error_ = error;
        return r;
    }

    /// Check if operation succeeded
    [[nodiscard]] constexpr bool IsOk() const { return error_ == ErrorCode::OK; }

    /// Get error code
    [[nodiscard]] constexpr ErrorCode GetError() const { return error_; }

   private:
    ErrorCode error_ = ErrorCode::OK;
};

}  // namespace abstractions
