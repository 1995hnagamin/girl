#include "location.hpp"
#include <boost/optional.hpp>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

std::string expand_path(const std::string &path);
void less(const Location &l);
std::vector<std::string> getEnvs(const char *key);
boost::optional<Location> find_target(const boost::optional<std::string> &glossary, const std::vector<std::string> &libpaths, const std::string &target);
boost::program_options::variables_map parse_commandline_arguments(int argc, char **argv);
std::string concat_arguments(const std::vector<std::string> &args);

template<typename T>
boost::optional<T> assoc(const std::string &option, const boost::program_options::variables_map &vm) {
  if (!vm.count(option)) {
    return boost::none;
  }
  return vm[option].as<T>();
}

