#pragma once

#include <vector>
#include <string>
#include <cstdint>

class AutoWOLImpl;

class AutoWOL {
  private:
    AutoWOLImpl* mImpl;
  public:
    AutoWOL(const std::vector<std::string>& targets, uint16_t nfGroup);
    ~AutoWOL();
};
