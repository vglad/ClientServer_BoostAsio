
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
    std::cout << "endpoint created. IP: " << ep.address().to_string()
              << ", port: " << ep.port() << '\n';

    auto ios  = io_service{};
    auto sock = cl.open_active_socket<ip::tcp>(ip::tcp::v4(), ios);
    std::cout << "socket opened: "
              << (sock.is_open() ? "successful" : "error") << '\n';

    auto it   = cl.resolve_host<ip::tcp::resolver>("loca", "54000", ios);
    for (auto it_end = ip::tcp::resolver::iterator{}; it != it_end; ++it) {
      std::cout << it->host_name()
                << " resolved with IP: " << it->endpoint().address().to_string()
                << ", port: " << it->endpoint().port() << '\n';
    }

  }
  catch (std::exception const &) {
    print_nested_exception();
  }
  return 0;
}