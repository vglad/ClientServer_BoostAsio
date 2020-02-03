#include <catch.hpp>

#include "Server.hpp"
#include "mocks/FakeServer.hpp"
#include "Common.hpp"

using namespace clientServer::server;
using namespace clientServer::mocks;
using namespace clientServer::common;
using namespace boost::asio;
using trompeloeil::_;

TEST_CASE("testing server create_endpoint", "[server]") {
  auto srv = Server{};

  SECTION("returns endpoint with specified port number, "
          "TCP protocol and IP address v4") {
    auto ep = srv.create_endpoint<ip::tcp::endpoint, ip::address_v4>(3333);
    REQUIRE(ep.port() == 3333);
    REQUIRE(ep.address().is_v4());
    REQUIRE(ep.protocol().protocol() == static_cast<int>(Protocol::TCP));
  }

  SECTION("returns endpoint with specified port number, "
          "TCP protocol and IP address v6") {
    auto ep = srv.create_endpoint<ip::tcp::endpoint, ip::address_v6>(3333);
    REQUIRE(ep.port() == 3333);
    REQUIRE(ep.address().is_v6());
    REQUIRE(ep.protocol().protocol() == static_cast<int>(Protocol::TCP));
  }

  SECTION("returns endpoint with specified port number, "
          "UDP protocol and IP address v4") {
    auto ep = srv.create_endpoint<ip::udp::endpoint, ip::address_v4>(3333);
    REQUIRE(ep.port() == 3333);
    REQUIRE(ep.address().is_v4());
    REQUIRE(ep.protocol().protocol() == static_cast<int>(Protocol::UDP));
  }

  SECTION("returns endpoint with specified port number, "
          "UDP protocol and IP address v6") {
    auto ep = srv.create_endpoint<ip::udp::endpoint, ip::address_v6>(3333);
    REQUIRE(ep.port() == 3333);
    REQUIRE(ep.address().is_v6());
    REQUIRE(ep.protocol().protocol() == static_cast<int>(Protocol::UDP));
  }

}

TEST_CASE("testing server open_passive_socket", "[server]") {
  SECTION("returns socket if opened successfully") {
    auto srv = Server{};

    SECTION("returns TCP, IPv4 socket") {
      auto sock = srv.open_passive_socket(ip::tcp::v4());
      REQUIRE(sock.is_open());
    }

    SECTION("returns TCP, IPv6 socket") {
      auto sock = srv.open_passive_socket(ip::tcp::v6());
      REQUIRE(sock.is_open());
    }

  }

  SECTION("returns error if opened with errors") {
    auto mock_srv = MockServer{};

    SECTION("throw error when open method failed for protocol TCP, IPv4") {
      REQUIRE_CALL(mock_srv, get_ec_value(_)).RETURN(22);
      REQUIRE_THROWS_WITH(
          mock_srv.open_passive_socket(ip::tcp::v4()),
          Catch::Contains("Failed to open the socket: [TCP, IPv4]. Error #: ")
      );
    }

    SECTION("throw error when open method failed for protocol TCP, IPv6") {
      REQUIRE_CALL(mock_srv, get_ec_value(_)).RETURN(22);
      REQUIRE_THROWS_WITH(
          mock_srv.open_passive_socket(ip::tcp::v6()),
          Catch::Contains("Failed to open the socket: [TCP, IPv6]. Error #: ")
      );
    }
  }
}

TEST_CASE("testing server get_ec_value", "[server]") {
  SECTION("get_ec_value returns proper value if no errors or empty") {
    auto fake_srv = FakeServer{};
    auto ec       = boost::system::error_code{};
    REQUIRE(fake_srv.get_ec_value(ec) == 0);
    REQUIRE(ec.message() == "Success");
  }

  SECTION("get_ec_value returns proper value if has value") {
    auto fake_srv = FakeServer{};
    auto ec       = boost::system::error_code{
        make_error_code(boost::system::errc::invalid_argument)
    };
    REQUIRE(fake_srv.get_ec_value(ec) == 22);
    REQUIRE(ec.message() == "Invalid argument");
  }
}