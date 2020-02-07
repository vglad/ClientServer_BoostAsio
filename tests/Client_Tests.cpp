#include <catch.hpp>

#include "Client.hpp"
#include "mocks/FakeClient.hpp"
#include "Common.hpp"

using namespace clientServer::client;
using namespace clientServer::mocks;
using namespace clientServer::common;
using namespace boost::asio;
using trompeloeil::_;

TEST_CASE("testing client create_endpoint", "[client]") {
  auto cl = Client{};

  SECTION("returns endpoint if parameters valid") {
    SECTION("returns endpoint if protocol is TCP and IP address v4") {
      auto ep = l<ip::tcp::endpoint, ip::address_v4>
                      ("127.0.0.1", 3333);
      REQUIRE(ep.port() == 3333);
      REQUIRE(ep.address().is_v4());
      REQUIRE(ep.address().to_string() == "127.0.0.1");
      REQUIRE(ep.protocol().protocol() == static_cast<int>(Protocol::TCP));
    }

    SECTION("returns endpoint if protocol is TCP and IP address v6") {
      auto ep = cl.create_endpoint<ip::tcp::endpoint, ip::address_v6>
                      ("fe80::4abd:ee8:f318:9528", 3333);
      REQUIRE(ep.port() == 3333);
      REQUIRE(ep.address().is_v6());
      REQUIRE(ep.address().to_string() == "fe80::4abd:ee8:f318:9528");
      REQUIRE(ep.protocol().protocol() == static_cast<int>(Protocol::TCP));
    }

    SECTION("returns endpoint if protocol is UDP and IP address v4") {
      auto ep = cl.create_endpoint<ip::udp::endpoint, ip::address_v4>
                      ("127.0.0.1", 3333);
      REQUIRE(ep.port() == 3333);
      REQUIRE(ep.address().is_v4());
      REQUIRE(ep.address().to_string() == "127.0.0.1");
      REQUIRE(ep.protocol().protocol() == static_cast<int>(Protocol::UDP));
    }

    SECTION("returns endpoint if protocol is UDP and IP address v6") {
      auto ep = cl.create_endpoint<ip::udp::endpoint, ip::address_v6>
                      ("fe80::4abd:ee8:f318:9528", 3333);
      REQUIRE(ep.port() == 3333);
      REQUIRE(ep.address().is_v6());
      REQUIRE(ep.address().to_string() == "fe80::4abd:ee8:f318:9528");
      REQUIRE(ep.protocol().protocol() == static_cast<int>(Protocol::UDP));
    }

  }

  SECTION("returns error if parameters invalid") {
    auto errNum = 22;
#ifdef WIN32
    errNum = 10022;
#endif

    SECTION("throw if protocol TCP, IPv4 and IP address not parsed") {
      REQUIRE_THROWS_AS(
          (cl.create_endpoint<ip::tcp::endpoint, ip::address_v4>("abcd", 3333)),
          std::invalid_argument
      );
      REQUIRE_THROWS_WITH(
          (cl.create_endpoint<ip::tcp::endpoint, ip::address_v4>("abcd", 3333)),
          Catch::Contains(concat("Failed to parce IP address: [IPv4, abcd]. ",
                                 "Error #: ", errNum))
      );
    }

    SECTION("throw if protocol TCP, IPv6 and IP address not parsed") {
      REQUIRE_THROWS_AS(
          (cl.create_endpoint<ip::tcp::endpoint,
                              ip::address_v6>("fe80::4abd::e65e8::f318:9528", 3333)),
          std::invalid_argument
      );
      REQUIRE_THROWS_WITH(
          (cl.create_endpoint<ip::tcp::endpoint,
                              ip::address_v6>("fe80::4abd::e65e8::f318:9528", 3333)),
          Catch::Contains(concat("Failed to parce ",
                                 "IP address: [IPv6, fe80::4abd::e65e8::f318:9528]. ",
                                 "Error #: ", errNum))
      );
    }

    SECTION("throw if protocol UDP, IPv4 and IP address not parsed") {
      REQUIRE_THROWS_AS(
          (cl.create_endpoint<ip::udp::endpoint,
                              ip::address_v4>("125.63.659.4", 3333)),
          std::invalid_argument
      );
      REQUIRE_THROWS_WITH(
          (cl.create_endpoint<ip::udp::endpoint,
                              ip::address_v4>("125.63.659.4", 3333)),
          Catch::Contains(concat("Failed to parce ",
                                 "IP address: [IPv4, 125.63.659.4]. ",
                                 "Error #: ", errNum))
      );
    }

    SECTION("throw if protocol UDP, IPv6 and IP address not parsed") {
      REQUIRE_THROWS_AS(
          (cl.create_endpoint<ip::udp::endpoint,
                              ip::address_v6>("fe80::4abd::e65e8::", 3333)),
          std::invalid_argument
      );
      REQUIRE_THROWS_WITH(
          (cl.create_endpoint<ip::udp::endpoint,
                              ip::address_v6>("fe80::4abd::e65e8::", 3333)),
          Catch::Contains(concat("Failed to parce ",
                                 "IP address: [IPv6, fe80::4abd::e65e8::]. ",
                                 "Error #: ", errNum))
      );
    }
  }

}

TEST_CASE("testing client open_active_socket", "[client]") {

  SECTION("returns socket if opened successfully") {
    auto cl = Client{};
    auto ios = io_service{};

    SECTION("returns TCP, IPv4 socket") {
      auto sock = cl.open_active_socket<ip::tcp>(ip::tcp::v4(), ios);
      REQUIRE(sock.is_open());
    }

    SECTION("returns TCP, IPv6 socket") {
      auto sock = cl.open_active_socket<ip::tcp>(ip::tcp::v6(), ios);
      REQUIRE(sock.is_open());
    }

    SECTION("returns UDP, IPv4 socket") {
      auto sock = cl.open_active_socket<ip::udp>(ip::udp::v4(), ios);
      REQUIRE(sock.is_open());
    }

    SECTION("returns UDP, IPv6 socket") {
      auto sock = cl.open_active_socket<ip::udp>(ip::udp::v6(), ios);
      REQUIRE(sock.is_open());
    }
  }

  SECTION("returns error if opened with errors") {
    auto mock_cl = MockClient{};
    auto ios = io_service{};

    SECTION("throw error when open method failed for protocol TCP, IPv4") {
      REQUIRE_CALL(mock_cl, get_ec_value(_))
      .RETURN(22);

      REQUIRE_THROWS_WITH(
          mock_cl.open_active_socket<ip::tcp>(ip::tcp::v4(), ios),
          Catch::Contains("Failed to open the socket: [TCP, IPv4]. Error #: ")
      );
    }

    SECTION("throw error when open method failed for protocol TCP, IPv6") {
      REQUIRE_CALL(mock_cl, get_ec_value(_))
      .RETURN(22);

      REQUIRE_THROWS_WITH(
          mock_cl.open_active_socket<ip::tcp>(ip::tcp::v6(), ios),
          Catch::Contains("Failed to open the socket: [TCP, IPv6]. Error #: ")
      );
    }

    SECTION("throw error when open method failed for protocol UDP, IPv4") {
      REQUIRE_CALL(mock_cl, get_ec_value(_))
      .RETURN(22);

      REQUIRE_THROWS_WITH(
          mock_cl.open_active_socket<ip::udp>(ip::udp::v4(), ios),
          Catch::Contains("Failed to open the socket: [UDP, IPv4]. Error #: ")
      );
    }

    SECTION("throw error when open method failed for protocol UDP, IPv6") {
      REQUIRE_CALL(mock_cl, get_ec_value(_))
      .RETURN(22);

      REQUIRE_THROWS_WITH(
          mock_cl.open_active_socket<ip::udp>(ip::udp::v6(), ios),
          Catch::Contains("Failed to open the socket: [UDP, IPv6]. Error #: ")
      );
    }
  }

}

TEST_CASE("testing client get_ec_value", "[client]") {
  SECTION("get_ec_value returns proper value if no errors or empty") {
    auto  msg = "Success";
#ifdef WIN32
    msg = "The operation completed successfully";
#endif
    auto fake_cl = FakeClient{};
    auto ec      = boost::system::error_code{};
    REQUIRE(fake_cl.get_ec_value(ec) == 0);
    REQUIRE(ec.message() == msg);
  }

  SECTION("get_ec_value returns proper value if has value") {
    auto  msg = "Invalid argument";
#ifdef WIN32
    msg = "Invalid argument";
#endif
    auto fake_cl = FakeClient{};
    auto ec      = boost::system::error_code{
                       make_error_code(boost::system::errc::invalid_argument)};
    REQUIRE(fake_cl.get_ec_value(ec) == 22);
    REQUIRE(ec.message() == msg);
  }

}


TEST_CASE("testing client resolve_host", "[client]") {
  auto cl  = Client{};
  auto ios = io_service{};
    
  SECTION("returns iterator to endpoints collection if resolved properly") {
    SECTION("returns iterator to endpoints collection, TCP protocol") {
      auto it = cl.resolve_host<ip::tcp::resolver>("localhost", "3333", ios);
      while (it->endpoint().address().is_v6()) { ++it; }
      REQUIRE(it->endpoint().port() == 3333);
      REQUIRE(it->endpoint().address().to_string() == "127.0.0.1");
      REQUIRE(
          it->endpoint().protocol().protocol() == static_cast<int>(Protocol::TCP));
    }

    SECTION("returns iterator to IP addresses collection, UDP protocol") {
      auto it = cl.resolve_host<ip::udp::resolver>("localhost", "3333", ios);
      while (it->endpoint().address().is_v6()) { ++it; }
      REQUIRE(it->endpoint().port() == 3333);
      REQUIRE(it->endpoint().address().to_string() == "127.0.0.1");
      REQUIRE(
          it->endpoint().protocol().protocol() == static_cast<int>(Protocol::UDP));
    }
  }
  

  /*
  //// these are working tests
  //// temporary disabled due to long timeout in syncronous resolve() call to unknown host names
  //// each test adds approximately 10 sec timeout

  SECTION("throw error if not resolved") {
    auto errNum = 1;
#ifdef WIN32
    errNum = 11001;
#endif

    SECTION("trow if protocol TCP") {
      REQUIRE_THROWS_AS(cl.resolve_host<ip::tcp::resolver>("loca", "3333", ios),
                        std::invalid_argument);
      //REQUIRE_THROWS_WITH(
      //    cl.resolve_host<ip::tcp::resolver>("loca", "3333", ios),
      //    Catch::Contains(concat("Failed to resolve IP address: [loca]. Error #: ",
      //                           errNum))
      //);
    }

    SECTION("trow if protocol UDP") {
    //  REQUIRE_THROWS_AS(cl.resolve_host<ip::udp::resolver>("loca", "3333", ios),
    //                    std::invalid_argument);
    //  REQUIRE_THROWS_WITH(
    //      cl.resolve_host<ip::udp::resolver>("loca", "3333", ios),
    //      Catch::Contains(concat("Failed to resolve IP address: [loca]. Error #: ",
    //                             errNum))
    //  );
    }
  }
  */

}
