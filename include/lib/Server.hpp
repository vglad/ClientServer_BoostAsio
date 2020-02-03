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
      constexpr Endpoint create_endpoint(uint16_t port_num) const noexcept;

     ip::tcp::acceptor open_passive_socket(ip::tcp const & ipVersion) const;

  protected:
    virtual int get_ec_value(boost::system::error_code const & ec) const noexcept;
  };

  template<typename Endpoint, typename IPAddressVer>
    constexpr Endpoint Server::create_endpoint(uint16_t port_num) const noexcept {
      return Endpoint(IPAddressVer::any(), port_num);
    }


}

#endif //CLIENTSERVER_INCLUDE_LIB_SERVER_HPP
