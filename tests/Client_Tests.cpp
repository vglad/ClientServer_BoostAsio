#include <catch.hpp>

#include "Client.hpp"
#include "Common.hpp"

using namespace clientServer::client;
using namespace clientServer::common;
using namespace boost::asio;

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

  SECTION("returns endpoint if parameters invalid") {

    SECTION("throw if protocol TCP, IPv4 and IP address not parsed") {
      REQUIRE_THROWS_MATCHES(
          (cl.create_endpoint<ip::tcp::endpoint, ip::address_v4>
              ("abcd", 3333)),
          std::invalid_argument,
          Catch::Matchers::Predicate<std::invalid_argument>(
              [](std::invalid_argument const & e) {
                return e.what() == std::string(
                    "Failed to parce IP address: [IPv4, abcd]. Error #: 22. "
                    "Message: Invalid argument");
              }
          )
      );
    }

    SECTION("throw if protocol TCP, IPv6 and IP address not parsed") {
      REQUIRE_THROWS_MATCHES(
          (cl.create_endpoint<ip::tcp::endpoint, ip::address_v6>
              ("fe80::4abd::e65e8::f318:9528", 3333)),
          std::invalid_argument,
          Catch::Matchers::Predicate<std::invalid_argument>(
              [](std::invalid_argument const & e) {
                return e.what() == std::string(
                    "Failed to parce IP address: "
                    "[IPv6, fe80::4abd::e65e8::f318:9528]. "
                    "Error #: 22. "
                    "Message: Invalid argument");
              }
          )
      );
    }

    SECTION("throw if protocol UDP, IPv4 and IP address not parsed") {
      REQUIRE_THROWS_MATCHES(
          (cl.create_endpoint<ip::udp::endpoint, ip::address_v6>
              ("125.63.659.4", 3333)),
          std::invalid_argument,
          Catch::Matchers::Predicate<std::invalid_argument>(
              [](std::invalid_argument const & e) {
                return e.what() == std::string(
                    "Failed to parce IP address: [IPv6, 125.63.659.4]. "
                    "Error #: 22. "
                    "Message: Invalid argument");
              }
          )
      );
    }

    SECTION("throw if protocol UDP, IPv6 and IP address not parsed") {
      REQUIRE_THROWS_MATCHES(
          (cl.create_endpoint<ip::udp::endpoint, ip::address_v6>
              ("fe80::4abd::e65e8::",3333)),
          std::invalid_argument,
          Catch::Matchers::Predicate<std::invalid_argument>(
              [](std::invalid_argument const & e) {
                return e.what() == std::string(
                    "Failed to parce IP address: [IPv6, fe80::4abd::e65e8::]. "
                    "Error #: 22. "
                    "Message: Invalid argument");
              }
          )
      );
    }

  }
}
