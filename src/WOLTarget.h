#pragma once

#include "boost/asio/ip/address.hpp"

#include <string>
#include <cstdlib>

class WOLTarget {
  private:
    boost::asio::ip::address mAddress;
  public:
    WOLTarget(const boost::asio::ip::address& address, uint16_t nfGroup);
    void wakeup();
};
