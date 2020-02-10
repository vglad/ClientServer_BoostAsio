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

    template<typename ProtocolT, typename IPAddrT>
      typename ProtocolT::endpoint
      create_endpoint(uint16_t port_num,
                      std::string const & raw_ip = std::string{}) const {
        try {
          auto ip_address = get_ip<IPAddrT>(raw_ip);
          return typename ProtocolT::endpoint(ip_address, port_num);
        }
        catch (...) {
          std::throw_with_nested(std::invalid_argument("Failed to create endpoint"));
        }
      }

    template<typename IPAddrT>
      IPAddrT get_ip(std::string const & raw_ip) const {
        if (raw_ip.empty()) { return IPAddrT::any(); }
        auto ec         = boost::system::error_code{};
        auto ip_address = IPAddrT::from_string(raw_ip, ec);
        if (ec.value() != 0) {
          std::throw_with_nested(std::invalid_argument(concat(
              "Failed to parse IP address: [",
              std::is_same_v<IPAddrT, ip::address_v4> ? "IPv4" : "IPv6", ", ",
              raw_ip, "]. Error #: ", ec.value(), ". Message: ", ec.message()
          )));
        }
        return ip_address;
      }

    template<typename ProtocolT, typename SocketT>
      SocketT open_socket(ProtocolT const & ipVer, io_service & ios) {
        auto socket    = SocketT(ios);
        auto ec        = boost::system::error_code{};
        socket.open(ipVer, ec);
        if (get_ec_value(ec) != 0) {
          std::throw_with_nested(std::runtime_error(concat(
              "Failed to open the socket: [",
              std::is_same_v<ProtocolT, ip::tcp> ? "TCP" : "UDP", ", ",
              ipVer == ProtocolT::v4()           ? "IPv4" : "IPv6",
              "]. Error #: ", ec.value(), ". Message: ", ec.message()
          )));
        }
        return socket;
      }

    template<typename SocketT, typename EndpointT>
      void bind_socket(SocketT & socket, EndpointT const & ep) {
        auto ec = boost::system::error_code{};
        socket.bind(ep, ec);
        if (get_ec_value(ec) != 0) {
          std::throw_with_nested(std::invalid_argument(concat(
              "Failed to bind socket. ",
              "Endpoint configuration: [Port: ",  ep.port() , ", ",
              "Address: ", ep.address().to_string(), ", ",
              ep.address().is_v4() ? "IPv4" : "IPv6", ", ",
              ep.protocol().protocol() == static_cast<int>(Protocol::TCP)
                                        ? "TCP" : "UDP", "]. ",
              "Error #: ", ec.value(), ". Message: ", ec.message()
          )));
        }
      }

//    template<typename IPAddrVer>
//      ip::tcp::acceptor
//      bind_socket(ip::tcp const & ipVersion,
//                          uint16_t port_num,
//                          io_service & _ios
//      ) const {
//        auto ep       = create_endpoint<ip::tcp::endpoint, IPAddrVer>(port_num);
//        auto acceptor = open_passive_socket(ipVersion, _ios);
//        auto ec       = boost::system::error_code{};
//        acceptor.bind(ep, ec);
//        if (get_ec_value(ec) != 0) {
//          std::throw_with_nested(std::invalid_argument(concat(
//              "Failed to bind acceptor socket: [TCP, ", port_num,
//              "]. Error #: ", ec.value(), ". Message: "
//          )));
//        }
//        return acceptor;
//      }


//    ip::tcp::acceptor
//    open_passive_socket(ip::tcp const & ipVersion, io_service & ios) const;


  protected:
    virtual int get_ec_value(boost::system::error_code const & ec) const noexcept;

  private:
    io_service ios_ = {};
  };






}

#endif //CLIENTSERVER_INCLUDE_LIB_SERVER_HPP
