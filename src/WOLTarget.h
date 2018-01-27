#pragma once

#include "WOLPacket.h"

#include <boost/asio/ip/udp.hpp>
#include <boost/asio/io_service.hpp>

#include <string>
#include <cstdlib>
#include <ostream>

class WOLTarget {
  public:
    using Socket = boost::asio::ip::udp::socket;
    using Endpoint = Socket::endpoint_type;
  private:
    Socket mSocket;
    WOLPacket mPacket;
    void notify(const boost::system::error_code& error,
                std::size_t bytes_transferred);
  public:
    WOLTarget(boost::asio::io_service& ios, const Socket::endpoint_type& ep, uint16_t nfGroup);
    void wakeup();
    Endpoint endpoint() const { return mSocket.remote_endpoint(); }
    MACAddress macAddress() const { return mPacket.macAddress(); }
};

std::ostream& operator<<(std::ostream& o, const WOLTarget& target);
