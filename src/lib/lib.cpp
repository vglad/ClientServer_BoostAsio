#include "lib.hpp"
#include <stdexcept>

using namespace library;

int64_t lib::addition(int32_t i, int32_t j) {
  return static_cast<int64_t>(i) + static_cast<int64_t>(j);
}

void lib::generate_throw() {
  throw std::runtime_error("Test exception catch");
}
