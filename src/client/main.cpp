#include "Precompiled.hpp"
#include "Client.hpp"

#include <iostream>

using namespace clientServer::client;
using namespace clientServer::detail;

int main() {
  auto cl = Client{};
  try {
    auto ep = cl.create_endpoint("127.0.0.1", 54000);
    std::cout << ep.address().to_string() << " "
              << ep.port() << '\n';
  }
  catch (std::exception const &) {
    print_nested_exception();
  }
  return 0;
}