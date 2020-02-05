
#include "Precompiled.hpp"
#include "Client.hpp"

#include <iostream>

int main() {
  using namespace clientServer::client;
  using namespace clientServer::detail;
  using namespace boost::asio;

  auto cl = Client{};
  try {
    auto ep = cl.create_endpoint<ip::tcp::endpoint, ip::address_v4>
                    ("127.0.0.1", 54000);
    std::cout << ep.address().to_string() << " " << ep.port() << '\n';

    auto sock = cl.open_active_socket<ip::tcp>(ip::tcp::v4());
    auto open = sock.is_open();
    std::cout << (open ? "opened" : "error") << '\n';

  }
  catch (std::exception const &) {
    print_nested_exception();
  }
  return 0;
}