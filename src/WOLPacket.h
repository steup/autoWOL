#pragma once

#include <array>
#include <ostream>

#include <boost/asio/buffer.hpp>

using MACAddress = std::array<uint8_t, 6>;

class WOLPacket {
  public:
    static constexpr const size_t idSize = 6;
    static constexpr const size_t numMacAddr = 16;
private:
  union {
    struct{
      uint8_t mId[idSize];
      uint8_t mMac[numMacAddr][MACAddress().size()];
    };
    uint8_t mData[sizeof(mId)+sizeof(mMac)];
  };
  public:
  WOLPacket();
  WOLPacket(const MACAddress& address);
  operator boost::asio::const_buffer() const { return boost::asio::const_buffer(mData, sizeof(mData)); }
  void macAddress(const MACAddress& mac);
  MACAddress macAddress() const;
};

std::ostream& operator<<(std::ostream& o, const MACAddress& mac);
std::ostream& operator<<(std::ostream& o, const WOLPacket& packet);
