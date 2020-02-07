
#include "Precompiled.hpp"
#include "Client.hpp"

#include <iostream>
#include <istream>
#include <string>

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

    auto it   = cl.resolve_host<ip::tcp::resolver>("example.com", "80", ios);
    for (auto it_end = ip::tcp::resolver::iterator{}; it != it_end; ++it) {
      std::cout << it->host_name()
                << " resolved with IP: " << it->endpoint().address().to_string()
                << ", port: " << it->endpoint().port() << '\n';
      if (it->endpoint().protocol().family() == boost::asio::ip::tcp::v4().family()) {
          sock.connect(*it);
          sock.send(boost::asio::buffer("GET / HTTP/1.1\r\nHost: www.example.com\r\n\r\n"));
          boost::asio::streambuf buf;
          auto bytes = boost::asio::read(sock, buf.prepare(2121));
          buf.commit(bytes);
          std::istream is{&buf};
          while (!is.eof()) {
              std::cout << static_cast<char>(is.get());
          }
      }
    }
  }
  catch (std::exception const &) {
    print_nested_exception();
  }
  return 0;
}