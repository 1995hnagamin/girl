#include "location.hpp"
#include "utils.hpp"
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <boost/optional.hpp>
#include <string>
#include <iostream>

int main(int argc, char **argv) {
  try {
    boost::program_options::variables_map vm(parse_commandline_arguments(argc, argv));
    if (!vm.count("query")) {
      std::string exec(argv[0]);
      std::cerr << "Usage: " + exec + " <word>" << std::endl;
      return 0;
    }
    std::string target(concat_arguments(vm["query"].as<std::vector<std::string>>()));
    boost::optional<std::string> glossary(assoc<std::string>("glossary", vm));
    boost::optional<Location> location(find_target(glossary, getEnvs("GIRLPATH"), target));
    if (location) {
      less(*location);
    } else {
      std::cerr << "not found: " + target << std::endl;
    }
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}
