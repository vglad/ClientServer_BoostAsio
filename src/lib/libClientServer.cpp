#include "libClientServer.hpp"
#include <stdexcept>
#include <boost/asio.hpp>

using namespace library;
using namespace boost;

int64_t libClientServer::addition(int32_t i, int32_t j) {
  return static_cast<int64_t>(i) + static_cast<int64_t>(j);
}

void libClientServer::generate_throw() {
  throw std::runtime_error("Test exception catch");
}

int Server::create_endpoint() {
  try {
      auto port_num   = uint16_t{ 3333 };
      auto ip_address = asio::ip::address{ asio::ip::address_v4::any() };
      auto ep         = asio::ip::tcp::endpoint(ip_address, port_num);
      return ep.port();
  }
  catch (std::exception const & e) {
    std::throw_with_nested(std::runtime_error(e.what()));
  }


  return 0;
}
