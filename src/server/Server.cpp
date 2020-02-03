#include "Server.hpp"

using namespace clientServer::server;

using Socket = ip::tcp::acceptor;
Socket Server::open_passive_socket(ip::tcp const & ipVersion) const {
  auto ios    = io_service{};
  auto socket = Socket(ios);
  auto ec     = boost::system::error_code{};
  socket.open(ipVersion, ec);

  if (get_ec_value(ec) != 0) {
    auto ipVer = ipVersion == ip::tcp::v4() ? "IPv4" : "IPv6";
    std::throw_with_nested(std::runtime_error(concat(
        "Failed to open the socket: [TCP, ", ipVer,
        "]. Error #: ", ec.value(), ". Message: ", ec.message()
    )));
  }
  return socket;
}

int Server::get_ec_value(boost::system::error_code const & ec) const noexcept {
  return ec.value();
}