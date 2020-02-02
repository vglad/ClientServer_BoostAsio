#ifndef CLIENTSERVER_INCLUDE_LIB_SERVER_HPP
#define CLIENTSERVER_INCLUDE_LIB_SERVER_HPP

#include <string>
#include <boost/asio.hpp>

namespace clientServer::server {
  using namespace boost;

  class Server {
  public:

    template<typename T>
    T create_endpoint(uint16_t port_num) {
      return T(asio::ip::address_v4::any(), port_num);
    }

  };

}

#endif //CLIENTSERVER_INCLUDE_LIB_SERVER_HPP
