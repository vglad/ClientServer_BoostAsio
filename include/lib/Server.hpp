#ifndef CLIENTSERVER_INCLUDE_LIB_SERVER_HPP
#define CLIENTSERVER_INCLUDE_LIB_SERVER_HPP

#include <string>
#include <boost/asio.hpp>

namespace clientServer::server {
  using namespace boost;

  class Server {
  public:
    asio::ip::tcp::endpoint create_endpoint(uint16_t port_num) const;

  };

}

#endif //CLIENTSERVER_INCLUDE_LIB_SERVER_HPP
