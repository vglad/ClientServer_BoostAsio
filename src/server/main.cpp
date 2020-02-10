#include "Precompiled.hpp"
#include "Server.hpp"

#include <iostream>
#include <memory>

int main() {
  using namespace clientServer::server;
  using namespace clientServer::detail;
  using namespace boost::asio;

  auto srv = Server{}; //fe80::4abd:ee8:f318:9528
  try {
//    auto epPtr = std::make_unique<ip::udp::endpoint>(
//                     srv.create_endpoint<ip::udp, ip::address_v6>(
//                         3333, "fe80::4abd:ee8:f318:9528"));
//    auto const & ep  = *epPtr;
//
//    std::cout << "endpoint created. IP: " << ep.address().to_string()
//              << ", port: " << ep.port() << '\n';
//
//    auto iosPtr  = std::make_unique<io_service>();
//    auto & ios   = *iosPtr;
//    auto sockPtr = std::make_unique<ip::udp::socket>(
//                       srv.open_socket<ip::udp, ip::udp::socket>
//                                      (ip::udp::v6(),ios));
//    auto & sock  = *sockPtr;
//
//    std::cout << "socket opened "
//              << (sock.is_open() ? "successfully." : "with errors!") << '\n';
//

//    srv.bind_socket<ip::udp::socket, ip::udp::endpoint>(sock, ep);
//
//        std::cout << "binding: "
//                  << (sock.is_open() ? "successful" : "error") << '\n';
//        if (sock.is_open()) {
//          std::cout << " IP address: " << sock.local_endpoint().address().to_string()
//                    << ", port: " << sock.local_endpoint().port()
//                    << ", protocol:" << '\n';
//        }

    // Step 1. Here we assume that the server application has
    // already obtained the protocol port number.
    unsigned short port_num = 3333;
    // Step 2. Creating an endpoint.
    ip::tcp::endpoint ep1(ip::address_v6::from_string
    ("fe80::6b87:a97a:d54c:129f%eth0"),
                          port_num);
    // Used by 'acceptor' class constructor.
    io_service ios1;
    // Step 3. Creating and opening an acceptor socket.
    ip::tcp::acceptor acceptor1(ios1, ep1.protocol());
    boost::system::error_code ec;
    // Step 4. Binding the acceptor socket.
    acceptor1.bind(ep1, ec);
    // Handling errors if any.
    if (ec.value() != 0) {
      // Failed to bind the acceptor socket. Breaking
      // execution.
      std::cout << "Failed to bind the acceptor socket."
                << "Error code = " << ec.value() << ". Message: "
                << ec.message() << "\n\n";
      return ec.value();
    } else {
      std::cout << "bind successful\n\n";
    }


  }
  catch (std::exception const &) {
    print_nested_exception();
  }
  return 0;
}
