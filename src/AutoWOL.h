#pragma once

#include <vector>
#include <string>
#include <cstdint>

class AutoWOLImpl;

class AutoWOL {
  private:
    AutoWOLImpl* mImpl;
  public:
    AutoWOL(uint16_t nfGroup, bool pedantic=false);
    void add(const std::string& target);
    ~AutoWOL();
    void run();
};
