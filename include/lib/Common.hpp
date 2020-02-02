#ifndef CLIENTSERVER_INCLUDE_LIB_COMMON_HPP
#define CLIENTSERVER_INCLUDE_LIB_COMMON_HPP

namespace clientServer::common {

  // boost::asio 1.72 protocols numbers IPPROTO_TCP or IPPROTO_UDP
  enum class Protocol {
    TCP = 6,
    UDP = 17
  };

}

#endif //CLIENTSERVER_INCLUDE_LIB_COMMON_HPP
