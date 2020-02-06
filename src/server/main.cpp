#include "Precompiled.hpp"
#include "Server.hpp"

#include <iostream>

int main() {
  using namespace clientServer::server;
  using namespace clientServer::detail;
  using namespace boost::asio;

  auto srv = Server{};
  try {
    auto ep = srv.create_endpoint<ip::tcp::endpoint, ip::address_v4>(54000);
    std::cout << ep.address().to_string() << " " << ep.port() << '\n';

    auto ios = io_service{};
    auto sock = srv.open_passive_socket(ip::tcp::v4(), ios);
    std::cout << (sock.is_open() ? "opened" : "error") << '\n';
  }
  catch (std::exception const &) {
    print_nested_exception();
  }
  return 0;
}
