#include "Client.hpp"

using namespace clientServer::client;

int Client::get_ec_value(boost::system::error_code const & ec) const noexcept {
  return ec.value();
}


