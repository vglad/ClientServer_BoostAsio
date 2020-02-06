#ifndef CLIENTSERVER_INCLUDE_LIB_COMMON_HPP
#define CLIENTSERVER_INCLUDE_LIB_COMMON_HPP

namespace clientServer::common {

  // boost::asio 1.72 protocols numbers IPPROTO_TCP or IPPROTO_UDP
  enum class Protocol {
    TCP = 6,
    UDP = 17
  };

  // boost::asio 1.72 protocols family() v4 and v6
   enum class IPVer{
     IPv4 = 2,
     IPv6 = 10
  };


}

#endif //CLIENTSERVER_INCLUDE_LIB_COMMON_HPP
