#ifndef CLIENTSERVER_INCLUDE_LIB_SERVER_HPP
#define CLIENTSERVER_INCLUDE_LIB_SERVER_HPP

#include <string>
#include <boost/asio.hpp>

namespace clientServer::server {

  class Server {
  public:

    template<typename Endpoint, typename IPAddressVer>
    Endpoint create_endpoint(uint16_t port_num) const noexcept;

  };

  template<typename Endpoint, typename IPAddressVer>
  Endpoint Server::create_endpoint(uint16_t port_num) const noexcept {
    return Endpoint(IPAddressVer::any(), port_num);
  }

}

#endif //CLIENTSERVER_INCLUDE_LIB_SERVER_HPP
