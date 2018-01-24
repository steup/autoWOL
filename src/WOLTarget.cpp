#include "WOLTarget.h"
#include <iostream>

using namespace std;
namespace ip = boost::asio::ip;

WOLTarget::WOLTarget(const ip::address& address, uint16_t nfGroup)
  : mAddress(address) {
  // TODO aquire mac address of target
}

void WOLTarget::wakeup() const {
  //TODO Implement WOL packet generation and transmission
}

ostream& operator<<(ostream& o, const WOLTarget& target) {
  return o << target.address();
}
