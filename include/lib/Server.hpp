#ifndef CLIENTSERVER_INCLUDE_LIB_SERVER_HPP
#define CLIENTSERVER_INCLUDE_LIB_SERVER_HPP

#include <string>
#include <boost/asio.hpp>
#include "StringUtils.hpp"
#include "Common.hpp"

namespace clientServer::server {
  using namespace clientServer::detail;
  using namespace clientServer::common;
  using namespace boost::asio;

  class Server {
  public:
    template<typename Endpoint, typename IPAddressVer>
      constexpr Endpoint
      create_endpoint(uint16_t port_num) const noexcept;

    ip::tcp::acceptor
    open_passive_socket(ip::tcp const & ipVersion, io_service & ios) const;

    template<typename SocketT, typename ProtocolT>
      SocketT open_socket(IPVer const & ipVer);

    template<typename IPAddrVer>
      ip::tcp::acceptor
      bind_socket(ip::tcp const & ipVersion,
                  uint16_t port_num,
                  io_service & _ios
      ) const;

  protected:
    virtual int get_ec_value(boost::system::error_code const & ec) const noexcept;

  private:
    io_service ios = {};
  };

  template<typename Endpoint, typename IPAddressVer>
    constexpr Endpoint Server::create_endpoint(uint16_t port_num) const noexcept {
      return Endpoint(IPAddressVer::any(), port_num);
    }

  template<typename SocketT, typename ProtocolT>
    SocketT Server::open_socket(IPVer const & ipVer) {
      auto socket    = SocketT(ios);
      auto ec        = boost::system::error_code{};
      auto ipVersion = (ipVer == IPVer::IPv4) ? ProtocolT::v4() : ProtocolT::v6();
      socket.open(ipVersion, ec);
      if (get_ec_value(ec) != 0) {
        auto const protocolType = std::is_same_v<ProtocolT, ip::tcp>
                                  ? "TCP" : "UDP";
        auto const ipV          = ipVer == IPVer::IPv4 ? "IPv4" : "IPv6";
        std::throw_with_nested(std::runtime_error(concat(
            "Failed to open the socket: [", protocolType, ", ", ipV,
            "]. Error #: ", ec.value(), ". Message: ", ec.message()
        )));
      }
      return socket;
    }


  template<typename IPAddrVer>
    ip::tcp::acceptor
    Server::bind_socket(ip::tcp const & ipVersion,
                        uint16_t port_num,
                        io_service & _ios
    ) const {
      auto ep       = create_endpoint<ip::tcp::endpoint, IPAddrVer>(port_num);
      auto acceptor = open_passive_socket(ipVersion, _ios);
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
