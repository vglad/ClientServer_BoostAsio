#include "Precompiled.hpp"
#include "libClientServer.hpp"

#include <iostream>

using namespace library;
using namespace detail;

int main() {
  try {
    auto l = libClientServer{};
    std::cout << l.addition(1, 41) << '\n';
    l.generate_throw();
  }
  catch (std::exception const &) {
     print_nested_exception();
  }
  return 0;
}
