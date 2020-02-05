#ifndef CLIENTSERVER_INCLUDE_LIB_SERVER_HPP
#define CLIENTSERVER_INCLUDE_LIB_SERVER_HPP

#include <string>
#include <boost/asio.hpp>
#include "StringUtils.hpp"

namespace clientServer::server {
  using namespace clientServer::detail;
  using namespace boost::asio;

  class Server {
  public:
    template<typename Endpoint, typename IPAddressVer>
      constexpr Endpoint
      create_endpoint(uint16_t port_num) const noexcept;

    ip::tcp::acceptor open_passive_socket(ip::tcp const & ipVersion) const;

    template<typename IPAddrVer>
      ip::tcp::acceptor
      bind_socket(ip::tcp const & ipVersion, uint16_t port_num) const;

  protected:
    virtual int get_ec_value(boost::system::error_code const & ec) const noexcept;
  };

  template<typename Endpoint, typename IPAddressVer>
    constexpr Endpoint Server::create_endpoint(uint16_t port_num) const noexcept {
      return Endpoint(IPAddressVer::any(), port_num);
    }

  template<typename IPAddrVer>
    ip::tcp::acceptor
    Server::bind_socket(ip::tcp const & ipVersion, uint16_t port_num) const {
      auto ep       = create_endpoint<ip::tcp::endpoint, IPAddrVer>(port_num);
      auto acceptor = open_passive_socket(ipVersion);
      auto ec       = boost::system::error_code{};
      acceptor.bind(ep, ec);
      if (get_ec_value(ec) != 0) {
        std::throw_with_nested(std::invalid_argument(concat(
            "Failed to bind acceptor socket: [TCP, ", port_num,
            "]. Error #: ", ec.value(), ". Message: "
        )));
      }


      return acceptor;
    }
}

#endif //CLIENTSERVER_INCLUDE_LIB_SERVER_HPP
