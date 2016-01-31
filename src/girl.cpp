#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib>
#include <boost/optional.hpp>
#include <boost/filesystem.hpp>
#include <wordexp.h>

boost::optional<int> find_section(std::ifstream &ifs, const std::string &target) {
  std::string query("# " + target);
  std::string row;
  int row_number = 0;
  while (std::getline(ifs, row)) {
    if (row == query) {
      return row_number;
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

int main(int argc, char **argv) {
  if (argc < 2) {
    std::string exec(argv[0]);
    std::cerr << "Usage: " + exec + " <word>" << std::endl;
    return 0;
  }
  std::string filepath = expand_path("~/local/share/girl/girl.md");
  std::ifstream ifs(filepath.data());

  if (ifs.fail()) {
    std::cerr << "Error on opening " + filepath << std::endl;
    return 0;
  }

  std::string target(argv[1]);
  boost::optional<int> row_number(find_section(ifs, target));

  if (row_number) {
    std::string cmd = "less +" + std::to_string(*row_number) + " " + filepath;
    system(cmd.data());
    return 0;
  } else {
    std::cerr << "not found: " + target << std::endl;
  }
}
