#include "WOLTarget.h"
#include <iostream>

using namespace std;
namespace ip = boost::asio::ip;

WOLTarget::WOLTarget(const ip::address& address, uint16_t nfGroup)
  : mAddress(address) {
  cout << "\t" << address << " using netfilter group " << nfGroup << endl;
}
