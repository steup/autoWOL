#include "AutoWOL.h"
#include "WOLTarget.h"

#include <boost/asio/ip/address.hpp>
#include <boost/asio/ip/udp.hpp>

#include <iostream>
#include <utility>
#include <map>

using namespace std;
namespace ip = boost::asio::ip;


struct AutoWOLImpl {
  map<ip::address, WOLTarget> targets;
  boost::asio::io_service ioService;

  AutoWOLImpl(const std::vector<std::string>& hosts, uint16_t nfGroup) {
    std::cout << "handling the following targets:" << std::endl;
    ip::udp::resolver resolver(ioService);
    for(const auto& host: hosts) {
      ip::udp::resolver::query q(host, string());
      try {
        ip::udp::resolver::iterator i=resolver.resolve(q);
        for(;i!=ip::udp::resolver::iterator();i++ ) {
          ip::address address = i->endpoint().address();
          WOLTarget wolTarget(address, nfGroup);
          targets.emplace(address, wolTarget);
        }
      } catch(...) {
        cout << "Could not resolve " << host << ": ignoring!" << endl;
      }
    }
    if(targets.empty()) {
      throw std::runtime_error("No hosts resolved: exiting!");
    }
  }
};


AutoWOL::AutoWOL(const std::vector<std::string>& targets, uint16_t nfGroup)
  : mImpl(new AutoWOLImpl(targets, nfGroup)) {
}

AutoWOL::~AutoWOL() {
  delete mImpl;
}
