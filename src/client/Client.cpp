#include "Client.hpp"
#include "StringUtils.hpp"

using namespace clientServer::client;
using namespace clientServer::detail;

asio::ip::tcp::endpoint Client::create_endpoint(std::string const & raw_ip,
                                                uint16_t port_num) const {
  auto err        = boost::system::error_code{};
  auto ip_address = asio::ip::address::from_string(raw_ip, err);
  if (err.value() != 0) {
    std::throw_with_nested(std::invalid_argument(concat(
        "Failed to parce IP address. Error #: ", err.value(),
        ". Message: ", err.message()))
    );
  }
  return asio::ip::tcp::endpoint(ip_address, port_num);
}
