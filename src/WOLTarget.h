#pragma once

#include "boost/asio/ip/address.hpp"

#include <string>
#include <cstdlib>
#include <ostream>

class WOLTarget {
  public:
    using Address = boost::asio::ip::address;
  private:
    Address mAddress;
  public:
    WOLTarget(const Address& address, uint16_t nfGroup);
    void wakeup() const;
    const Address& address() const { return mAddress; }
};

std::ostream& operator<<(std::ostream& o, const WOLTarget& target);
