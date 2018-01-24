#include "Error.h"

#include <iostream>

using namespace std;

void  error::check(const error_code& ec, bool pedantic) {
  if(ec)
    if(pedantic)
      BOOST_THROW_EXCEPTION(error::Error(ec));
    else
      cout << ec.message() << endl;
}
