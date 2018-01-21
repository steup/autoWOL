#include "AutoWOL.h"

#include <iostream>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/positional_options.hpp>

namespace po = boost::program_options;

int main(int argc, char** argv) {
  uint16_t nfGroup, logLevel;
  po::options_description opts(std::string(argv[0])+" <options> <target IP or hostname>");
  opts.add_options()
    ("daemon,d", "fork to background")
    ("group,g", po::value<uint16_t>(&nfGroup)->default_value(0), "netfilter group to use for communication")
    ("logLevel,l", po::value<uint16_t>(&logLevel)->default_value(3), "logging level 0-7")
    ("targets,t", po::value<std::vector<std::string>>()->composing(), "target IPs and hostnames");
  po::options_description cmdOnlyOpts("commandline only");
  cmdOnlyOpts.add_options()
    ("help,h", "print this help message")
    ("version,v", "print version information");
  po::positional_options_description p;
  p.add("targets", -1);
  opts.add(cmdOnlyOpts);
  po::variables_map var;
  po::store(po::parse_command_line(argc, argv, opts), var);
  po::store(po::command_line_parser(argc, argv).options(opts).positional(p).run(), var);
  if(!var.count("help") && var.count("version")) {
    std::cout << "autoWOL " << VERSION << std::endl;
    return 0;
  }
  if(!var.count("targets") || var.count("help")) {
    std::cout << opts;
    return -1;
  }
  std::cout << "AutoWOL starting" << std::endl;
  AutoWOL(var["targets"].as<std::vector<std::string>>(), nfGroup);
  std::cout << "AutoWOL terminated" << std::endl;
  return 0;
}
