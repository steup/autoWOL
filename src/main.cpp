#include "AutoWOL.h"
#include "Version.h"
#include "Error.h"

#include <iostream>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/positional_options.hpp>
#include <boost/exception/diagnostic_information.hpp>

namespace po = boost::program_options;
using namespace std;

using boost::diagnostic_information;

int main(int argc, char** argv) {
  uint16_t nfGroup;
  unsigned int logLevel;
  po::options_description opts("autoWOL <options> <target IP or hostname>");
  opts.add_options()
    ("daemon,d", "fork to background")
    ("group,g", po::value<uint16_t>(&nfGroup)->default_value(0), "netfilter group to use for communication")
    ("logLevel,l", po::value<unsigned int >(&logLevel)->default_value(3), "logging level 0-7")
    ("targets,t", po::value<vector<string>>()->composing(), "target IPs and hostnames")
    ("pedantic,p", "treat all warnings as errors");
  po::options_description cmdOnlyOpts("commandline only");
  cmdOnlyOpts.add_options()
    ("help,h", "print this help message")
    ("version,v", "print version information");
  po::positional_options_description p;
  p.add("targets", -1);
  opts.add(cmdOnlyOpts);
  po::variables_map var;
  try {
  po::store(po::parse_command_line(argc, argv, opts), var);
  po::store(po::command_line_parser(argc, argv).options(opts).positional(p).run(), var);
  if(var.count("help")) {
    cout << opts << endl;
    return -1;
  }
  if(var.count("version")) {
    cout << "autoWOL " << version << endl;
    return 0;
  }
    if(!var.count("targets"))
      BOOST_THROW_EXCEPTION(error::Error("No target hosts specified!"));
    cout << "AutoWOL starting" << endl;
    AutoWOL daemon(nfGroup, var.count("pedantic"));
    for(const string& host: var["targets"].as<std::vector<std::string>>())
      daemon.add(host);
    daemon.run();
  } catch(const boost::bad_lexical_cast& e){
    cout << e.what() << endl << opts;
    return -1;
  } catch(const error::Error& e) {
    cout << diagnostic_information(e) << endl;
    return -1;
  }
  cout << "AutoWOL terminated" << endl;
  return 0;
}
