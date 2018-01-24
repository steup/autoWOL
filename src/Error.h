#pragma once

#include <boost/system/system_error.hpp>
#include <boost/system/error_code.hpp>
#include <stdexcept>
#include <boost/exception/exception.hpp>
#include <boost/exception/info.hpp>
#include <boost/throw_exception.hpp>

namespace error {
  using boost::system::error_code;

  struct Error : public std::runtime_error, public boost::exception {
    Error(const std::runtime_error& error) : std::runtime_error(error) {}
    Error(const char* msg) : std::runtime_error(msg) {}
    Error(const boost::system::error_code& ec) : std::runtime_error(std::system_error(ec)) {}
  };
  struct HostTag {};
  using Host = boost::error_info<HostTag, const char*>;
  void  check(const error_code& ec, bool pedantic=false);
}
