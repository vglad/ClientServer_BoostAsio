#include "Precompiled.hpp"
#include "Server.hpp"

#include <iostream>
#include <memory>

int main() {
  using namespace clientServer::server;
  using namespace clientServer::detail;
  using namespace boost::asio;

  auto srv = Server{};
  try {

    auto ep = srv.create_endpoint<ip::tcp::endpoint, ip::address_v4>(54000);
    std::cout << "endpoint created. IP: " << ep.address().to_string()
              << ", port: " << ep.port() << '\n';

    auto iosPtr = std::make_unique<io_service>();
    auto& ios = *iosPtr;
    auto sock = srv.open_passive_socket(ip::tcp::v4(), ios);
    auto clientPtr = std::make_unique<ip::tcp::socket>(ios);
    auto& client = *clientPtr;
    sock.async_accept(client, [clientPtr = std::move(clientPtr), iosPtr = std::move(iosPtr)] (const auto& ec) {
        if (ec)
            return;

    });
    std::cout << "socket opened: "
              << (sock.is_open() ? "successful" : "error") << '\n';

    auto acceptor = srv.bind_socket<ip::address_v4>(ip::tcp::v4(), 54000, ios);
    std::cout << "binding: "
              << (acceptor.is_open() ? "successful" : "error") << '\n';
    if (acceptor.is_open()) {
      std::cout << " IP address: " << acceptor.local_endpoint().address().to_string()
                << ", port: " << acceptor.local_endpoint().port() 
                << ", protocol: TCP" << '\n';
    }


  }
  catch (std::exception const &) {
    print_nested_exception();
  }
  return 0;
}
