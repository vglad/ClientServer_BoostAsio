#ifndef CLIENTSERVER_INCLUDE_LIB_CLIENT_HPP
#define CLIENTSERVER_INCLUDE_LIB_CLIENT_HPP

#include <string>
#include <boost/asio.hpp>

namespace clientServer::client {
  using namespace boost;

  class Client {
  public:
    asio::ip::tcp::endpoint create_endpoint(std::string const & raw_ip,
                                            uint16_t port_num) const;
  };

}

#endif //CLIENTSERVER_INCLUDE_LIB_CLIENT_HPP
