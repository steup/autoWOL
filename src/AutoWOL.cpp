#include "AutoWOL.h"
#include "WOLTarget.h"
#include "Error.h"

#include <boost/asio/ip/address.hpp>
#include <boost/asio/ip/udp.hpp>
#include <boost/asio/system_timer.hpp>
#include <boost/bind.hpp>


#include <iostream>
#include <utility>
#include <map>
#include <chrono>

using namespace std;
namespace ip = boost::asio::ip;
namespace sys = boost::system;
using namespace boost::placeholders;
using namespace std::chrono;

struct AutoWOLImpl {
  bool mPedantic;
  uint16_t mNfGroup;
  map<ip::address, WOLTarget> targets;
  boost::asio::io_service ioService;
  boost::asio::system_timer mTimer;

  AutoWOLImpl(uint16_t nfGroup, bool pedantic)
    : mPedantic(pedantic), mNfGroup(nfGroup), mTimer(ioService) {
    //TODO netfilter link
  }

  void add(const string& host) {
    ip::udp::resolver resolver(ioService);
    ip::udp::resolver::query q(host, "9");
    sys::error_code ec;
    ip::udp::resolver::iterator i=resolver.resolve(q, ec);
    if(ec) {
      if(mPedantic)
        BOOST_THROW_EXCEPTION(error::Error(ec) << error::Host(host.c_str()));
      else
        cout << ec.message() << " \"" << host << "\"" << endl;
    } else
    for(;i!=ip::udp::resolver::iterator();i++ )
        targets.emplace( i->endpoint().address(),
                         WOLTarget(ioService, i->endpoint(), mNfGroup));
  }

  void log() const{
    cout << "Handling the following targets on nfgroup " << mNfGroup << endl;
    for(const auto& targetPair: targets)
      cout << "\t" << targetPair.second << endl;
  }

  void trigger(const sys::error_code& ec) {
    mTimer.expires_from_now(seconds(60));
    error::check(ec);
    log();
    for(auto& targetPair : targets) {
      targetPair.second.wakeup();
    }
    mTimer.async_wait(boost::bind(&AutoWOLImpl::trigger, this, _1));
  };

  void run() {
    trigger(error::error_code());
    while(true) {
      error::error_code ec;
      ioService.run(ec);
      error::check(ec);
    }
  }

};


AutoWOL::AutoWOL(uint16_t nfGroup, bool pedantic)
  : mImpl(new AutoWOLImpl(nfGroup, pedantic)) {
}

AutoWOL::~AutoWOL() {
  delete mImpl;
}

void AutoWOL::add(const string& host) {
  mImpl->add(host);
}

void AutoWOL::run() {
  mImpl->run();
}
