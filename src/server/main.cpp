#include "Precompiled.hpp"
#include "libClientServer.hpp"

#include <iostream>

using namespace library;
using namespace detail;

int main() {
  try {
    auto s = Server{};
    //std::cout << l.addition(1, 41) << '\n';
    //l.generate_throw();

    std::cout << "endpoint created on port: " << s.create_endpoint() << '\n';

  }
  catch (std::exception const &) {
    print_nested_exception();
  }
  return 0;
}
