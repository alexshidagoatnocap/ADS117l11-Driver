#include <cstdint>
namespace abstractions {

enum class Status : uint8_t { ABSTRACT_OK, ABSTRACT_INIT_FAIL, ABSTRACT_ERROR };

enum class PinState : bool { LOW = false, HIGH = true };

} // namespace abstractions
