#ifndef CLIENTSERVER_INCLUDE_LIB_CLIENT_HPP
#define CLIENTSERVER_INCLUDE_LIB_CLIENT_HPP

#include <string>
#include <boost/asio.hpp>

#include "StringUtils.hpp"

namespace clientServer::client {
  using namespace clientServer::detail;
  using namespace boost::asio;

  class Client {
  public:

    template<typename Endpoint, typename IPAddressVer>
    Endpoint create_endpoint(std::string const & raw_ip, uint16_t port_num) const;

  };

  template<typename Endpoint, typename IPAddressVer>
  Endpoint Client::create_endpoint(std::string const & raw_ip, uint16_t port_num) const {
    auto err        = boost::system::error_code{};
    auto ip_address = IPAddressVer::from_string(raw_ip, err);
    if (err.value() != 0) {
      auto ipType = std::is_same_v<IPAddressVer, ip::address_v4>
                    ? "IPv4" : "IPv6";
      std::throw_with_nested(std::invalid_argument(concat(
          "Failed to parce IP address: [", ipType, ", ", raw_ip,
          "]. Error #: ", err.value(),
          ". Message: ", err.message()))
      );
    }
    return Endpoint(ip_address, port_num);
  }

}

#endif //CLIENTSERVER_INCLUDE_LIB_CLIENT_HPP
