#include <catch.hpp>

#include "Server.hpp"
#include "mocks/FakeServer.hpp"
#include "Common.hpp"

using namespace clientServer::server;
using namespace clientServer::mocks;
using namespace clientServer::common;
using namespace boost::asio;
using trompeloeil::_;

TEST_CASE("testing server get_ec_value", "[server]") {
  SECTION("get_ec_value returns proper value if no errors or empty") {
    auto msg = "Success";
#ifdef WIN32
    msg = "The operation completed successfully";
#endif

    auto fake_srv = FakeServer{};
    auto ec       = boost::system::error_code{};
    REQUIRE(fake_srv.get_ec_value(ec) == 0);
    REQUIRE(ec.message() == msg);
  }

  SECTION("get_ec_value returns proper value if has value") {
    auto fake_errNum = 22;
    auto fake_srv    = FakeServer{};
    auto ec          = boost::system::error_code{
        make_error_code(boost::system::errc::invalid_argument)
    };
    REQUIRE(fake_srv.get_ec_value(ec) == fake_errNum);
    REQUIRE(ec.message() == "Invalid argument");
  }
}

TEST_CASE("testing get_ip", "[server]") {
  auto srv = Server{};

  SECTION("returns proper IP type") {
    SECTION("returns proper IP type if IPv4 and properly parsed") {
      auto ip = srv.get_ip<ip::address_v4>("127.0.0.1");
      REQUIRE(ip.to_string() == "127.0.0.1");
      REQUIRE(ip.any() == ip::address_v4());
    }

    SECTION("returns proper IP type if IPv6 and properly parsed") {
      auto ip = srv.get_ip<ip::address_v6>("fe80::4abd:ee8:f318:9528");
      REQUIRE(ip.to_string() == "fe80::4abd:ee8:f318:9528");
      REQUIRE(ip.any() == ip::address_v6());
    }

  }

  SECTION("returns error if parameters invalid") {
    auto errNum = 22;
#ifdef WIN32
    errNum = 10022;
#endif

    SECTION("throw if protocol IPv4 and not parsed") {
      REQUIRE_THROWS_AS(srv.get_ip<ip::address_v4>("127.0.0"),
                        std::invalid_argument);
      REQUIRE_THROWS_WITH(
          srv.get_ip<ip::address_v4>("127.0.0"),
          Catch::Contains(concat("Failed to parse IP address: [IPv4, 127.0.0]. ",
                                 "Error #: ", errNum))
      );
    }

    SECTION("throw if protocol IPv6 and not parsed") {
      REQUIRE_THROWS_AS(srv.get_ip<ip::address_v6>("fe80::4a::"),
                        std::invalid_argument);
      REQUIRE_THROWS_WITH(
          srv.get_ip<ip::address_v6>("fe80::4a::"),
          Catch::Contains(concat("Failed to parse IP address: [IPv6, fe80::4a::]. ",
                                 "Error #: ", errNum))
      );
    }
  }
}

TEST_CASE("testing server create_endpoint", "[server]") {
  auto srv = Server{};

  SECTION("returns endpoint if parameters valid") {
    SECTION("returns endpoint with specified port number, "
            "TCP protocol and IP address v4, unspecified") {
      auto ep = srv.create_endpoint<ip::tcp, ip::address_v4>(3333);
      REQUIRE(ep.port() == 3333);
      REQUIRE(ep.address().is_v4());
      REQUIRE(ep.protocol().protocol() == ip::tcp::v4().protocol());
    }

    SECTION("returns endpoint with specified port number, "
            "TCP protocol and IP address v4, 127.0.0.1") {
      auto ep = srv.create_endpoint<ip::tcp, ip::address_v4>(
          3333, "127.0.0.1");
      REQUIRE(ep.port() == 3333);
      REQUIRE(ep.address().is_v4());
      REQUIRE(ep.address().to_string() == "127.0.0.1");
      REQUIRE(ep.protocol().protocol() == ip::tcp::v4().protocol());
    }

    SECTION("returns endpoint with specified port number, "
            "TCP protocol and IP address v6, unspecified") {
      auto ep = srv.create_endpoint<ip::tcp, ip::address_v6>(3333);
      REQUIRE(ep.port() == 3333);
      REQUIRE(ep.address().is_v6());
      REQUIRE(ep.protocol().protocol() == ip::tcp::v6().protocol());
    }

    SECTION("returns endpoint with specified port number, "
            "TCP protocol and IP address v6, fe80::4abd:ee8:f318:9528") {
      auto ep = srv.create_endpoint<ip::tcp, ip::address_v6>(
          3333, "fe80::4abd:ee8:f318:9528");
      REQUIRE(ep.port() == 3333);
      REQUIRE(ep.address().is_v6());
      REQUIRE(ep.address().to_string() == "fe80::4abd:ee8:f318:9528");
      REQUIRE(ep.protocol().protocol() == ip::tcp::v6().protocol());
    }

    SECTION("returns endpoint with specified port number, "
            "UDP protocol and IP address v4, unspecified") {
      auto ep = srv.create_endpoint<ip::udp, ip::address_v4>(3333);
      REQUIRE(ep.port() == 3333);
      REQUIRE(ep.address().is_v4());
      REQUIRE(ep.protocol().protocol() == ip::udp::v4().protocol());
    }

    SECTION("returns endpoint with specified port number, "
            "UDP protocol and IP address v4, 127.0.0.1") {
      auto ep = srv.create_endpoint<ip::udp, ip::address_v4>(
          3333, "127.0.0.1");
      REQUIRE(ep.port() == 3333);
      REQUIRE(ep.address().is_v4());
      REQUIRE(ep.address().to_string() == "127.0.0.1");
      REQUIRE(ep.protocol().protocol() == ip::udp::v4().protocol());
    }

    SECTION("returns endpoint with specified port number, "
            "UDP protocol and IP address v6, unspecified") {
      auto ep = srv.create_endpoint<ip::udp, ip::address_v6>(3333);
      REQUIRE(ep.port() == 3333);
      REQUIRE(ep.address().is_v6());
      REQUIRE(ep.protocol().protocol() == ip::udp::v6().protocol());
    }

    SECTION("returns endpoint with specified port number, "
            "UDP protocol and IP address v6, fe80::4abd:ee8:f318:9528") {
      auto ep = srv.create_endpoint<ip::udp, ip::address_v6>(
          3333, "fe80::4abd:ee8:f318:9528");
      REQUIRE(ep.port() == 3333);
      REQUIRE(ep.address().is_v6());
      REQUIRE(ep.address().to_string() == "fe80::4abd:ee8:f318:9528");
      REQUIRE(ep.protocol().protocol() == ip::udp::v6().protocol());
    }
  }

  SECTION("returns endpoint if parameters valid") {

    SECTION("throw if protocol TCP, IPv4 and IP can not be parsed") {
      REQUIRE_THROWS_AS(
          (srv.create_endpoint<ip::tcp, ip::address_v4>(3333, "127.0.0")),
          std::invalid_argument);
      REQUIRE_THROWS_WITH(
          (srv.create_endpoint<ip::tcp, ip::address_v4>(3333, "127.0.0")),
          Catch::Contains(concat("Failed to create endpoint"))
      );
    }

    SECTION("throw if protocol TCP, IPv6 and IP can not be parsed") {
      REQUIRE_THROWS_AS(
          (srv.create_endpoint<ip::tcp, ip::address_v6>(3333, "fe80::4abd:")),
          std::invalid_argument);
      REQUIRE_THROWS_WITH(
          (srv.create_endpoint<ip::tcp, ip::address_v6>(3333, "fe80::4abd:")),
          Catch::Contains(concat("Failed to create endpoint"))
      );
    }

    SECTION("throw if protocol UDP, IPv4 and IP can not be parsed") {
      REQUIRE_THROWS_AS(
          (srv.create_endpoint<ip::udp, ip::address_v4>(3333, "127.0.0")),
          std::invalid_argument);
      REQUIRE_THROWS_WITH(
          (srv.create_endpoint<ip::udp, ip::address_v4>(3333, "127.0.0")),
          Catch::Contains(concat("Failed to create endpoint"))
      );
    }

    SECTION("throw if protocol UDP, IPv6 and IP can not be parsed") {
      REQUIRE_THROWS_AS(
          (srv.create_endpoint<ip::udp, ip::address_v6>(3333, "fe80::4abd:")),
          std::invalid_argument);
      REQUIRE_THROWS_WITH(
          (srv.create_endpoint<ip::udp, ip::address_v6>(3333, "fe80::4abd:")),
          Catch::Contains(concat("Failed to create endpoint"))
      );
    }
  }
}

TEST_CASE("testing server open_socket", "[server]") {

  SECTION("returns socket if opened successfully") {
    auto cl     = Server{};
    auto iosPtr = std::make_unique<io_service>();
    auto & ios = *iosPtr;

    SECTION("returns TCP, IPv4 acceptor") {
      auto sock = cl.open_socket<ip::tcp, ip::tcp::acceptor>
                        (ip::tcp::v4(), ios);
      REQUIRE(sock.is_open());
      REQUIRE(
          sock.local_endpoint().protocol().protocol() ==
          ip::tcp::v4().protocol());
      REQUIRE(
          sock.local_endpoint().protocol().family() ==
          ip::tcp::v4().family());
    }

    SECTION("returns TCP, IPv6 acceptor") {
      auto sock = cl.open_socket<ip::tcp, ip::tcp::acceptor>
                        (ip::tcp::v6(), ios);
      REQUIRE(sock.is_open());
      REQUIRE(
          sock.local_endpoint().protocol().protocol() ==
          ip::tcp::v6().protocol());
      REQUIRE(
          sock.local_endpoint().protocol().family() ==
          ip::tcp::v6().family());
    }

    SECTION("returns TCP, IPv4 socket") {
      auto sock = cl.open_socket<ip::tcp, ip::tcp::socket>
                        (ip::tcp::v4(), ios);
      REQUIRE(sock.is_open());
      REQUIRE(
          sock.local_endpoint().protocol().protocol() ==
          ip::tcp::v4().protocol());
      REQUIRE(
          sock.local_endpoint().protocol().family() ==
          ip::tcp::v4().family());
    }

    SECTION("returns TCP, IPv6 socket") {
      auto sock = cl.open_socket<ip::tcp, ip::tcp::socket>
                        (ip::tcp::v6(), ios);
      REQUIRE(sock.is_open());
      REQUIRE(
          sock.local_endpoint().protocol().protocol() ==
          ip::tcp::v6().protocol());
      REQUIRE(
          sock.local_endpoint().protocol().family() ==
          ip::tcp::v6().family());
    }

    SECTION("returns UDP, IPv4 socket") {
      auto sock = cl.open_socket<ip::udp, ip::udp::socket>
                        (ip::udp::v4(), ios);
      REQUIRE(sock.is_open());
      REQUIRE(
          sock.local_endpoint().protocol().protocol() ==
          ip::udp::v4().protocol());
      REQUIRE(
          sock.local_endpoint().protocol().family() ==
          ip::udp::v4().family());
    }

    SECTION("returns UDP, IPv6 socket") {
      auto sock = cl.open_socket<ip::udp, ip::udp::socket>
                        (ip::udp::v6(), ios);
      REQUIRE(sock.is_open());
      REQUIRE(
          sock.local_endpoint().protocol().protocol() ==
          ip::udp::v6().protocol());
      REQUIRE(
          sock.local_endpoint().protocol().family() ==
          ip::udp::v6().family());
    }

  }

  SECTION("returns error if opened with errors") {
    auto mock_srv = MockServer{};
    auto iosPtr   = std::make_unique<io_service>();
    auto & ios = *iosPtr;

    SECTION("throw error when open_socket failed for acceptor, "
            "protocol TCP, IPv4") {
      REQUIRE_CALL(mock_srv, get_ec_value(_)).RETURN(22);
      REQUIRE_THROWS_AS(
          (mock_srv.open_socket<ip::tcp, ip::tcp::acceptor>(ip::tcp::v4(), ios)),
          std::runtime_error
      );
      REQUIRE_CALL(mock_srv, get_ec_value(_)).RETURN(22);
      REQUIRE_THROWS_WITH(
          (mock_srv.open_socket<ip::tcp, ip::tcp::acceptor>(ip::tcp::v4(), ios)),
          Catch::Contains("Failed to open the socket: [TCP, IPv4]. Error #: ")
      );
    }

    SECTION("throw error when open_socket failed for acceptor, "
            "protocol TCP, IPv6") {
      REQUIRE_CALL(mock_srv, get_ec_value(_)).RETURN(22);
      REQUIRE_THROWS_AS(
          (mock_srv.open_socket<ip::tcp, ip::tcp::acceptor>(ip::tcp::v6(), ios)),
          std::runtime_error
      );
      REQUIRE_CALL(mock_srv, get_ec_value(_)).RETURN(22);
      REQUIRE_THROWS_WITH(
          (mock_srv.open_socket<ip::tcp, ip::tcp::acceptor>(ip::tcp::v6(), ios)),
          Catch::Contains("Failed to open the socket: [TCP, IPv6]. Error #: ")
      );
    }

    SECTION("throw error when open_socket failed for active socket, "
            "protocol TCP, IPv4") {
      REQUIRE_CALL(mock_srv, get_ec_value(_)).RETURN(22);
      REQUIRE_THROWS_AS(
          (mock_srv.open_socket<ip::tcp, ip::tcp::socket>(ip::tcp::v4(), ios)),
          std::runtime_error
      );
      REQUIRE_CALL(mock_srv, get_ec_value(_)).RETURN(22);
      REQUIRE_THROWS_WITH(
          (mock_srv.open_socket<ip::tcp, ip::tcp::socket>(ip::tcp::v4(), ios)),
          Catch::Contains("Failed to open the socket: [TCP, IPv4]. Error #: ")
      );
    }

    SECTION("throw error when open_socket failed for active socket, "
            "protocol TCP, IPv6") {
      REQUIRE_CALL(mock_srv, get_ec_value(_)).RETURN(22);
      REQUIRE_THROWS_AS(
          (mock_srv.open_socket<ip::tcp, ip::tcp::socket>(ip::tcp::v6(), ios)),
          std::runtime_error
      );
      REQUIRE_CALL(mock_srv, get_ec_value(_)).RETURN(22);
      REQUIRE_THROWS_WITH(
          (mock_srv.open_socket<ip::tcp, ip::tcp::socket>(ip::tcp::v6(), ios)),
          Catch::Contains("Failed to open the socket: [TCP, IPv6]. Error #: ")
      );
    }

    SECTION("throw error when open_socket failed for active socket, "
            "protocol UDP, IPv4") {
      REQUIRE_CALL(mock_srv, get_ec_value(_)).RETURN(22);
      REQUIRE_THROWS_AS(
          (mock_srv.open_socket<ip::udp, ip::udp::socket>(ip::udp::v4(), ios)),
          std::runtime_error
      );
      REQUIRE_CALL(mock_srv, get_ec_value(_)).RETURN(22);
      REQUIRE_THROWS_WITH(
          (mock_srv.open_socket<ip::udp, ip::udp::socket>(ip::udp::v4(), ios)),
          Catch::Contains("Failed to open the socket: [UDP, IPv4]. Error #: ")
      );
    }

    SECTION("throw error when open_socket failed for active socket, "
            "protocol UDP, IPv6") {
      REQUIRE_CALL(mock_srv, get_ec_value(_)).RETURN(22);
      REQUIRE_THROWS_AS(
          (mock_srv.open_socket<ip::udp, ip::udp::socket>(ip::udp::v6(), ios)),
          std::runtime_error
      );
      REQUIRE_CALL(mock_srv, get_ec_value(_)).RETURN(22);
      REQUIRE_THROWS_WITH(
          (mock_srv.open_socket<ip::udp, ip::udp::socket>(ip::udp::v6(), ios)),
          Catch::Contains("Failed to open the socket: [UDP, IPv6]. Error #: ")
      );
    }

  }

}


TEST_CASE("testing bind", "[server]") {
  auto srv = Server{};

  SECTION("successful bind with valid parameters") {

    SECTION("binds acceptor and endpoint with TCP protocol, IPv4") {
      auto epPtr = std::make_unique<ip::tcp::endpoint>(
          srv.create_endpoint<ip::tcp, ip::address_v4>(3333, "127.0.0.1"));
      auto & ep = *epPtr;
      auto iosPtr = std::make_unique<io_service>();
      auto & ios = *iosPtr;
      auto sockPtr = std::make_unique<ip::tcp::acceptor>(
          srv.open_socket<ip::tcp, ip::tcp::acceptor>(
              ip::tcp::v4(), ios));
      auto & sock = *sockPtr;

      REQUIRE_NOTHROW(
          (srv.bind_socket<ip::tcp::acceptor, ip::tcp::endpoint>(sock, ep)));
      REQUIRE(sock.local_endpoint().protocol() == ip::tcp::v4());
      REQUIRE(sock.local_endpoint().address().is_v4());
      REQUIRE(sock.local_endpoint().address().to_string() == "127.0.0.1");
      REQUIRE(sock.local_endpoint().port() == 3333);
    }

    SECTION("binds acceptor and endpoint with TCP protocol, IPv6") {
      auto epPtr = std::make_unique<ip::tcp::endpoint>(
          srv.create_endpoint<ip::tcp, ip::address_v6>(
              3333, "fe80::6b87:a97a:d54c:129f%eth0"));
      auto & ep = *epPtr;
      auto iosPtr = std::make_unique<io_service>();
      auto & ios = *iosPtr;
      auto sockPtr = std::make_unique<ip::tcp::acceptor>(
          srv.open_socket<ip::tcp, ip::tcp::acceptor>(
              ip::tcp::v6(), ios));
      auto & sock = *sockPtr;
      REQUIRE_NOTHROW(
          (srv.bind_socket<ip::tcp::acceptor, ip::tcp::endpoint>(sock, ep)));
      REQUIRE(sock.local_endpoint().protocol() == ip::tcp::v6());
      REQUIRE(sock.local_endpoint().address().is_v6());
      REQUIRE(sock.local_endpoint().address().to_string() ==
      "fe80::6b87:a97a:d54c:129f%eth0");
      REQUIRE(sock.local_endpoint().port() == 3333);
    }

    SECTION("binds active socket and endpoint with TCP protocol, IPv4") {
      auto epPtr = std::make_unique<ip::tcp::endpoint>(
          srv.create_endpoint<ip::tcp, ip::address_v4>(3333));
      auto & ep = *epPtr;
      auto iosPtr = std::make_unique<io_service>();
      auto & ios = *iosPtr;
      auto sockPtr = std::make_unique<ip::tcp::socket>(
          srv.open_socket<ip::tcp, ip::tcp::socket>(
              ip::tcp::v4(), ios));
      auto & sock = *sockPtr;

      REQUIRE_NOTHROW(
          (srv.bind_socket<ip::tcp::socket, ip::tcp::endpoint>(sock, ep)));
      REQUIRE(sock.local_endpoint().protocol() == ip::tcp::v4());
      REQUIRE(sock.local_endpoint().address().is_v4());
      REQUIRE(sock.local_endpoint().port() == 3333);
    }

    SECTION("binds active socket and endpoint with TCP protocol, IPv6") {
      auto epPtr = std::make_unique<ip::tcp::endpoint>(
          srv.create_endpoint<ip::tcp, ip::address_v6>(
              3333, "fe80::6b87:a97a:d54c:129f%eth0"));
      auto & ep = *epPtr;
      auto iosPtr = std::make_unique<io_service>();
      auto & ios = *iosPtr;
      auto sockPtr = std::make_unique<ip::tcp::socket>(
          srv.open_socket<ip::tcp, ip::tcp::socket>(
              ip::tcp::v6(), ios));
      auto & sock = *sockPtr;
      REQUIRE_NOTHROW(
          (srv.bind_socket<ip::tcp::socket, ip::tcp::endpoint>(sock, ep)));
      REQUIRE(sock.local_endpoint().protocol() == ip::tcp::v6());
      REQUIRE(sock.local_endpoint().address().is_v6());
      REQUIRE(
          sock.local_endpoint().address().to_string() ==
          "fe80::6b87:a97a:d54c:129f%eth0");
      REQUIRE(sock.local_endpoint().port() == 3333);
    }

    SECTION("binds active socket and endpoint with UDP protocol, IPv4") {
      auto epPtr = std::make_unique<ip::udp::endpoint>(
          srv.create_endpoint<ip::udp, ip::address_v4>(3333));
      auto & ep = *epPtr;
      auto iosPtr = std::make_unique<io_service>();
      auto & ios = *iosPtr;
      auto sockPtr = std::make_unique<ip::udp::socket>(
          srv.open_socket<ip::udp, ip::udp::socket>(
              ip::udp::v4(), ios));
      auto & sock = *sockPtr;

      REQUIRE_NOTHROW(
          (srv.bind_socket<ip::udp::socket, ip::udp::endpoint>(sock, ep)));
      REQUIRE(sock.local_endpoint().protocol() == ip::udp::v4());
      REQUIRE(sock.local_endpoint().address().is_v4());
      REQUIRE(sock.local_endpoint().port() == 3333);
    }

    SECTION("binds active socket and endpoint with UDP protocol, IPv6") {
      auto epPtr = std::make_unique<ip::udp::endpoint>(
          srv.create_endpoint<ip::udp, ip::address_v6>(3333));
      auto & ep = *epPtr;
      auto iosPtr = std::make_unique<io_service>();
      auto & ios = *iosPtr;
      auto sockPtr = std::make_unique<ip::udp::socket>(
          srv.open_socket<ip::udp, ip::udp::socket>(
              ip::udp::v6(), ios));
      auto & sock = *sockPtr;
      REQUIRE_NOTHROW(
          (srv.bind_socket<ip::udp::socket, ip::udp::endpoint>(sock, ep)));
      REQUIRE(sock.local_endpoint().protocol() == ip::udp::v6());
      REQUIRE(sock.local_endpoint().address().is_v6());
      REQUIRE(sock.local_endpoint().port() == 3333);
    }
  }
  //
  //  SECTION("throw if parameters invalid") {
  //    auto errNum = 97;
  //
  //    SECTION("throw if IP version in endpoint and address mismatch") {
  //#ifdef WIN32
  //      errNum = 10047;
  //#endif
  //      REQUIRE_THROWS_AS(srv.bind_socket<ip::address_v6>(ip::tcp::v4(), 3333, ios),
  //                        std::invalid_argument);
  //      REQUIRE_THROWS_WITH(
  //          srv.bind_socket<ip::address_v6>(ip::tcp::v4(), 3333, ios),
  //          Catch::Contains(concat("Failed to bind acceptor socket: [TCP, 3333]. ",
  //                                 "Error #: ", errNum, ". Message: "))
  //      );
  //
  //      errNum = 22;
  //#ifdef WIN32
  //      errNum = 10014;
  //#endif
  //      REQUIRE_THROWS_AS(srv.bind_socket<ip::address_v4>(ip::tcp::v6(), 3333, ios),
  //                        std::invalid_argument);
  //      REQUIRE_THROWS_WITH(
  //          srv.bind_socket<ip::address_v4>(ip::tcp::v6(), 3333, ios),
  //          Catch::Contains(concat("Failed to bind acceptor socket: [TCP, 3333]. ",
  //                                 "Error #: ", errNum, ". Message: "))
  //      );
  //    }
  //  }
}

/*
//TEST_CASE("testing server open_passive_socket", "[server]") {
//  SECTION("returns socket if opened successfully") {
//    auto srv = Server{};
//    auto ios = io_service{};
//
//    SECTION("returns TCP, IPv4 socket") {
//      auto sock = srv.open_passive_socket(ip::tcp::v4(), ios);
//      REQUIRE(sock.is_open());
//    }
//
//    SECTION("returns TCP, IPv6 socket") {
//      auto sock = srv.open_passive_socket(ip::tcp::v6(), ios);
//      REQUIRE(sock.is_open());
//    }
//
//  }
//
//  SECTION("returns error if opened with errors") {
//    auto mock_srv = MockServer{};
//    auto ios      = io_service{};
//
//    SECTION("throw error when open method failed for protocol TCP, IPv4") {
//      REQUIRE_CALL(mock_srv, get_ec_value(_)).RETURN(22);
//      REQUIRE_THROWS_WITH(
//          mock_srv.open_passive_socket(ip::tcp::v4(), ios),
//          Catch::Contains("Failed to open the socket: [TCP, IPv4]. Error #: ")
//      );
//    }
//
//    SECTION("throw error when open method failed for protocol TCP, IPv6") {
//      REQUIRE_CALL(mock_srv, get_ec_value(_)).RETURN(22);
//      REQUIRE_THROWS_WITH(
//          mock_srv.open_passive_socket(ip::tcp::v6(), ios),
//          Catch::Contains("Failed to open the socket: [TCP, IPv6]. Error #: ")
//      );
//    }
//  }
//}
*/
