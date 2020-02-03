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
      auto ep = cl.create_endpoint<ip::tcp::endpoint, ip::address_v4>
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
    SECTION("throw if protocol TCP, IPv4 and IP address not parsed") {
      REQUIRE_THROWS_MATCHES(
          (
              cl.create_endpoint<ip::tcp::endpoint, ip::address_v4>
                    ("abcd", 3333)),
          std::invalid_argument,
          Catch::Matchers::Predicate<std::invalid_argument>(
              [](std::invalid_argument const & e) {
                return e.what() == std::string(
                    "Failed to parce IP address: [IPv4, abcd]. Error #: 22. "
                    "Message: Invalid argument"
                );
              }
          )
      );
    }

    SECTION("throw if protocol TCP, IPv6 and IP address not parsed") {
      REQUIRE_THROWS_MATCHES(
          (
              cl.create_endpoint<ip::tcp::endpoint, ip::address_v6>
                    ("fe80::4abd::e65e8::f318:9528", 3333)),
          std::invalid_argument,
          Catch::Matchers::Predicate<std::invalid_argument>(
              [](std::invalid_argument const & e) {
                return e.what() == std::string(
                    "Failed to parce IP address: "
                    "[IPv6, fe80::4abd::e65e8::f318:9528]. "
                    "Error #: 22. "
                    "Message: Invalid argument"
                );
              }
          )
      );
    }

    SECTION("throw if protocol UDP, IPv4 and IP address not parsed") {
      REQUIRE_THROWS_MATCHES(
          (
              cl.create_endpoint<ip::udp::endpoint, ip::address_v6>
                    ("125.63.659.4", 3333)),
          std::invalid_argument,
          Catch::Matchers::Predicate<std::invalid_argument>(
              [](std::invalid_argument const & e) {
                return e.what() == std::string(
                    "Failed to parce IP address: [IPv6, 125.63.659.4]. "
                    "Error #: 22. "
                    "Message: Invalid argument"
                );
              }
          )
      );
    }

    SECTION("throw if protocol UDP, IPv6 and IP address not parsed") {
      REQUIRE_THROWS_MATCHES(
          (
              cl.create_endpoint<ip::udp::endpoint, ip::address_v6>
                    ("fe80::4abd::e65e8::", 3333)),
          std::invalid_argument,
          Catch::Matchers::Predicate<std::invalid_argument>(
              [](std::invalid_argument const & e) {
                return e.what() == std::string(
                    "Failed to parce IP address: [IPv6, fe80::4abd::e65e8::]. "
                    "Error #: 22. "
                    "Message: Invalid argument"
                );
              }
          )
      );
    }
  }
}

TEST_CASE("testing client open_active_socket", "[client]") {
  SECTION("returns socket if opened successfully") {
    auto cl = Client{};

    SECTION("returns TCP, IPv4 socket") {
      auto sock = cl.open_active_socket<ip::tcp>(ip::tcp::v4());
      REQUIRE(sock.is_open());
    }

    SECTION("returns TCP, IPv6 socket") {
      auto sock = cl.open_active_socket<ip::tcp>(ip::tcp::v6());
      REQUIRE(sock.is_open());
    }

    SECTION("returns UDP, IPv4 socket") {
      auto sock = cl.open_active_socket<ip::udp>(ip::udp::v4());
      REQUIRE(sock.is_open());
    }

    SECTION("returns UDP, IPv6 socket") {
      auto sock = cl.open_active_socket<ip::udp>(ip::udp::v6());
      REQUIRE(sock.is_open());
    }
  }

  SECTION("returns error if opened with errors") {
    auto mock_cl = MockClient{};

    SECTION("throw error when open method failed for protocol TCP, IPv4") {
      REQUIRE_CALL(mock_cl, get_ec_value(_))
      .RETURN(22);

      REQUIRE_THROWS_WITH(
          mock_cl.open_active_socket<ip::tcp>(ip::tcp::v4()),
          Catch::Contains("Failed to open the socket: [TCP, IPv4]. Error #: ")
      );
    }

    SECTION("throw error when open method failed for protocol TCP, IPv6") {
      REQUIRE_CALL(mock_cl, get_ec_value(_))
      .RETURN(22);

      REQUIRE_THROWS_WITH(
          mock_cl.open_active_socket<ip::tcp>(ip::tcp::v6()),
          Catch::Contains("Failed to open the socket: [TCP, IPv6]. Error #: ")
      );
    }

    SECTION("throw error when open method failed for protocol UDP, IPv4") {
      REQUIRE_CALL(mock_cl, get_ec_value(_))
      .RETURN(22);

      REQUIRE_THROWS_WITH(
          mock_cl.open_active_socket<ip::udp>(ip::udp::v4()),
          Catch::Contains("Failed to open the socket: [UDP, IPv4]. Error #: ")
      );
    }

    SECTION("throw error when open method failed for protocol UDP, IPv6") {
      REQUIRE_CALL(mock_cl, get_ec_value(_))
      .RETURN(22);

      REQUIRE_THROWS_WITH(
          mock_cl.open_active_socket<ip::udp>(ip::udp::v6()),
          Catch::Contains("Failed to open the socket: [UDP, IPv6]. Error #: ")
      );
    }
  }
}

TEST_CASE("testing client get_ec_value", "[client]") {
  SECTION("get_ec_value returns proper value if no errors or empty") {
    auto fake_cl = FakeClient{};
    auto ec      = boost::system::error_code{};
    REQUIRE(fake_cl.get_ec_value(ec) == 0);
    REQUIRE(ec.message() == "Success");
  }

  SECTION("get_ec_value returns proper value if has value") {
    auto fake_cl = FakeClient{};
    auto ec      = boost::system::error_code{
        make_error_code(boost::system::errc::invalid_argument)
    };
    REQUIRE(fake_cl.get_ec_value(ec) == 22);
    REQUIRE(ec.message() == "Invalid argument");
  }
}

