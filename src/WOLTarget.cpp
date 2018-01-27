#include "WOLTarget.h"
#include "Error.h"

#include <iostream>
#include <regex>
#include <fstream>
#include <boost/bind.hpp>

using namespace std;
namespace ip = boost::asio::ip;
using boost::asio::const_buffers_1;
using boost::placeholders::_1;
using boost::placeholders::_2;
using boost::bind;

static uint8_t fromHex(char c) {
  return c-(c<'a'?'0':'a');
}

static MACAddress getMac(const boost::asio::ip::address& address) {
  ifstream arpCache("/proc/net/arp");
  ostringstream os;
  os << address;
  string regExp= os.str()+".*((?:[[:xdigit:]]{2}:){5}[[:xdigit:]])";
  regex parseCache=regex(regExp);
  while(!arpCache.eof()) {
    char line[255];
    arpCache.getline(line, sizeof(line));
    cmatch res;
    if(!regex_search(line, res, parseCache)||res.size()!=2)
      continue;
    MACAddress mac;
    istringstream is(res[1]);
    size_t i=0;
    for(uint8_t& v: mac) {
      uint16_t temp;
      char c;
      is >> hex >> temp >> c;
      v=temp;
    }
    cout << "Mapping of " << address << " to " << mac << " found" << endl;
    return mac;
  }
  BOOST_THROW_EXCEPTION(error::Error("MAC Address lookup failed") << error::Host(os.str().c_str()));
}

WOLTarget::WOLTarget(boost::asio::io_service& ios, const Socket::endpoint_type& ep, uint16_t nfGroup)
  : mSocket(ios), mPacket(getMac(ep.address()))  {
    mSocket.connect(ep);
  // TODO: create netfilter rules
}

void WOLTarget::notify(const boost::system::error_code& error,
                       std::size_t bytes_transferred) {
  cout << "Send " << mPacket << " from " << mSocket.local_endpoint() << " to "
       << mSocket.remote_endpoint() << ": result " << error.message() << ", "
       << bytes_transferred << " bytes send" << endl;
}

void WOLTarget::wakeup() {
  auto func = bind(&WOLTarget::notify, this, _1, _2);
  mSocket.async_send(const_buffers_1(mPacket), func);
}

ostream& operator<<(ostream& o, const WOLTarget& target) {
  return o << target.endpoint().address() << "(" << target.macAddress() << ")";
}
