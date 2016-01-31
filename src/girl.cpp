#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib>
#include <boost/optional.hpp>
#include <boost/filesystem.hpp>
#include <wordexp.h>
#include "location.hpp"

boost::optional<Location> find_section(const std::string &filepath, const std::string &target) {
  std::ifstream ifs(filepath.data());

  if (ifs.fail()) {
    return boost::none;
  }

  std::string query("# " + target);
  std::string row;
  int row_number = 0;
  while (std::getline(ifs, row)) {
    if (row == query) {
      return Location(filepath, row_number);
    }
    row_number++;
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

int main(int argc, char **argv) {
  if (argc < 2) {
    std::string exec(argv[0]);
    std::cerr << "Usage: " + exec + " <word>" << std::endl;
    return 0;
  }

  std::string filepath = expand_path("~/local/share/girl/girl.md");
  std::string target(argv[1]);
  boost::optional<Location> location(find_section(filepath, target));

  if (location) {
    less(*location);
  } else {
    std::cerr << "not found: " + target << std::endl;
  }
}
