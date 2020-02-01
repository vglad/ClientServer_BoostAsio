#include "Server.hpp"

using namespace clientServer::server;

asio::ip::tcp::endpoint Server::create_endpoint(uint16_t port_num) const {
  auto ip_address = asio::ip::address{ asio::ip::address_v4::any() };
  return asio::ip::tcp::endpoint(ip_address, port_num);
}
