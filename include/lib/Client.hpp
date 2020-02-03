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
      constexpr Endpoint
      create_endpoint(std::string const & raw_ip, uint16_t port_num) const;

    template<typename SocketType>
      constexpr typename SocketType::socket
      open_active_socket(SocketType const & ipVersion) const;

  protected:
    virtual int get_ec_value(boost::system::error_code const & ec) const noexcept;
  };

  template<typename Endpoint, typename IPAddressVer>
    constexpr Endpoint
    Client::create_endpoint(std::string const & raw_ip, uint16_t port_num) const {
      auto ec         = boost::system::error_code{};
      auto ip_address = IPAddressVer::from_string(raw_ip, ec);
      if (ec.value() != 0) {
        auto ipVer = std::is_same_v<IPAddressVer, ip::address_v4>
                     ? "IPv4" : "IPv6";
        std::throw_with_nested(std::invalid_argument(concat(
            "Failed to parce IP address: [", ipVer, ", ", raw_ip,
            "]. Error #: ", ec.value(), ". Message: ", ec.message()
        )));
      }
      return Endpoint(ip_address, port_num);
    }

  template<typename SocketType>
    constexpr typename SocketType::socket
    Client::open_active_socket(SocketType const & ipVersion) const {
      auto ios    = io_service{};
      auto socket = typename SocketType::socket(ios);
      auto ec     = boost::system::error_code{};
      socket.open(ipVersion, ec);
      if (get_ec_value(ec) != 0) {
        auto const protocolType = std::is_same_v<SocketType, ip::tcp>
                                  ? "TCP" : "UDP";
        auto const ipVer        = ipVersion == SocketType::v4() ? "IPv4" : "IPv6";
        std::throw_with_nested(std::runtime_error(concat(
            "Failed to open the socket: [", protocolType, ", ", ipVer,
            "]. Error #: ", ec.value(), ". Message: ", ec.message()
        )));
      }
      return socket;
    }

}

#endif //CLIENTSERVER_INCLUDE_LIB_CLIENT_HPP
