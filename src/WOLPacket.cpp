#include "WOLPacket.h"
#include <iomanip>

using namespace std;

WOLPacket::WOLPacket() {
  memset(mData, 0xFF, sizeof(mData));
}

WOLPacket::WOLPacket(const MACAddress& address) {
  memset(mId, 0xFF, sizeof(mId));
  macAddress(address);
}

void WOLPacket::macAddress(const MACAddress& mac) {
  for(size_t i=0; i < numMacAddr; i++)
    for(size_t j=0; j < MACAddress().size(); j++)
      mMac[i][j] = mac[j];
}

MACAddress WOLPacket::macAddress() const {
  MACAddress mac;
  for(size_t i=0; i < MACAddress().size(); i++)
    mac[i] = mMac[0][i];
  return mac;
}

ostream& operator<<(ostream& o, const MACAddress& mac) {
  size_t i=0;
  auto f=o.flags();
  for(uint8_t v : mac)
    o << hex << setw(2) << setfill('0') << (uint16_t)v << ((++i!=MACAddress().size())?":":"");
  o.setf(f);
  return o;
}

ostream& operator<<(ostream& o, const WOLPacket& packet) {
  return o << "WOL Packet to " << packet.macAddress();
}
