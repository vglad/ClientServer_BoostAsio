#include "Precompiled.hpp"
#include "Server.hpp"

#include <iostream>

using namespace clientServer::server;
using namespace clientServer::detail;

int main() {
  auto srv = Server{};
  try {
    auto ep = srv.create_endpoint(54000);
    std::cout << ep.address().to_string() << " "
              << ep.port() << '\n';
  }
  catch (std::exception const &) {
    print_nested_exception();
  }
  return 0;
}
