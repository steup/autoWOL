#include "AutoWOL.h"

#include <iostream>

struct AutoWOLImpl {
  AutoWOLImpl(const std::vector<std::string>& targets, uint16_t nfGroup) {
    std::cout << "using netfilter group " << nfGroup << std::endl;
    std::cout << "handling the following targets:" << std::endl;
    for(const auto& t: targets)
      std::cout << "\t" << t << std::endl;
  }
};


AutoWOL::AutoWOL(const std::vector<std::string>& targets, uint16_t nfGroup)
  : mImpl(new AutoWOLImpl(targets, nfGroup)) {
}

AutoWOL::~AutoWOL() {
  delete mImpl;
}
