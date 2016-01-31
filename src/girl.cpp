#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib>
#include <boost/optional.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <wordexp.h>
#include "location.hpp"

boost::optional<Location> find_section(const boost::filesystem::path &filepath, const std::string &target) {
  std::ifstream ifs(filepath.string());

  if (ifs.fail()) {
    return boost::none;
  }

  std::string query("# " + target);
  std::string row;
  int row_number = 1;
  while (std::getline(ifs, row)) {
    if (row == query) {
      return Location(filepath, row_number);
    }
    row_number++;
  }
  return boost::none;
}

boost::optional<Location> find_glossary(const boost::filesystem::path &glospath, const std::string &target) {
  namespace fs = boost::filesystem;
  fs::path glossary(glospath.string());
  for (fs::recursive_directory_iterator iter = fs::recursive_directory_iterator(glossary);
      iter != fs::end(iter);
      iter++) {
    fs::path filepath(*iter);
    boost::optional<Location> loc(find_section(filepath, target));
    if (loc) {
      return loc;
    }
  }
  return boost::none;
}

boost::optional<Location> find_library(const std::string &libpath, const std::string &target) {
  namespace fs = boost::filesystem;
  fs::path library(libpath);
  for (fs::directory_iterator iter = fs::directory_iterator(library);
      iter != fs::end(iter);
      iter++) {
    fs::path glospath(*iter);
    boost::optional<Location> loc(find_glossary(glospath, target));
    if (loc) {
      return loc;
    }
  }
  return boost::none;
}

std::string expand_path(std::string path) {
  wordexp_t p;
  int flags = 0;
  int expansion = wordexp(path.data(), &p, flags);
  if (expansion == 0) {
    std::string expanded_path(p.we_wordv[0]);
    wordfree(&p);
    return expanded_path;
  }
  throw;
}

void less(Location l) {
  std::string cmd = "less";
  cmd += " +" + std::to_string(l.get_row_number());
  cmd += " " + l.get_path().string();
  system(cmd.data());
}

std::vector<std::string> getEnvs(const char *key) {
  std::string value(std::getenv(key));
  std::vector<std::string> vec;
  boost::algorithm::split(vec, value, boost::algorithm::is_any_of(":"));
  return vec;
}

int main(int argc, char **argv) {
  if (argc < 2) {
    std::string exec(argv[0]);
    std::cerr << "Usage: " + exec + " <word>" << std::endl;
    return 0;
  }
  std::string target(argv[1]);

  std::vector<std::string> libpaths(getEnvs("GIRLPATH"));
  bool found = false;
  for (std::string libpath : libpaths) {
    boost::optional<Location> location(find_library(libpath, target));
    if (location) {
      less(*location);
      found = true;
      break;
    }
  }

  if (not found) {
    std::cerr << "not found: " + target << std::endl;
  }
}
