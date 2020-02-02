#ifndef CLIENTSERVER_INCLUDE_LIB_CLIENT_HPP
#define CLIENTSERVER_INCLUDE_LIB_CLIENT_HPP

#include <string>
#include <boost/asio.hpp>

#include "StringUtils.hpp"

namespace clientServer::client {
  using namespace boost;
  using namespace detail;

  class Client {
  public:

    template<typename T>
    T create_endpoint(std::string const & raw_ip, uint16_t port_num) const {
      auto err        = boost::system::error_code{};
      auto ip_address = asio::ip::address::from_string(raw_ip, err);
      if (err.value() != 0) {
        std::throw_with_nested(std::invalid_argument(concat(
            "Failed to parce IP address. Error #: ", err.value(),
            ". Message: ", err.message()))
        );
      }
      return T(ip_address, port_num);
    }

  };

}

#endif //CLIENTSERVER_INCLUDE_LIB_CLIENT_HPP
