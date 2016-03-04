#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib>
#include <functional>
#include <algorithm>
#include <boost/optional.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/program_options.hpp>
#include <wordexp.h>
#include "location.hpp"

bool does_contain_alias(const std::string &line, const std::string &alias) {
  using namespace std::placeholders;
  if (line.empty() || line[0] != '=') {
    return false;
  }
  std::string aliasline(line.substr(1));
  std::vector<std::string> aliases;
  boost::algorithm::split(aliases, aliasline, boost::algorithm::is_any_of(","));
  //return std::any_of(aliases.begin(), aliases.end(), std::bind(std::equal_to, _1, alias));
  for (const std::string &str : aliases) { // XXX
    if (str == alias) {
      return true;
    }
  }
  return false;
}

bool is_section_title(const std::string &line) {
  return line.length() > 1 && line[0] == '#' && line[1] != '#';
}

boost::optional<Location> find_section(const boost::filesystem::path &filepath, const std::string &target) {
  std::ifstream ifs(filepath.string());

  if (ifs.fail()) {
    return boost::none;
  }

  std::string query("# " + target);
  std::string row;
  int row_number = 1;
  bool is_previous_section = false; // whether previous line is section title ("# title...")
  while (std::getline(ifs, row)) {
    if (row == query) {
      return Location(filepath, row_number);
    }
    if (is_previous_section && does_contain_alias(row, target)) {
      return Location(filepath, row_number - 1);
    }
    is_previous_section = is_section_title(row);
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

boost::optional<boost::filesystem::path> search_libpaths(const std::string &glossary, const std::vector<std::string> &libpaths) {
  namespace fs = boost::filesystem;
  for (std::string libpath : libpaths) {
    for (fs::directory_iterator iter = fs::directory_iterator(libpath);
        iter != fs::end(iter);
        iter++) {
      fs::path glospath(*iter);
      if (glospath.filename() == glossary) {
        return glospath;
      }
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

boost::optional<Location> find_libraries(const std::vector<std::string> &libpaths, const std::string &target) {
  for (std::string libpath : libpaths) {
    boost::optional<Location> location(find_library(libpath, target));
    if (location) {
      return location;
    }
  }
  return boost::none;
}

std::string expand_path(const std::string &path) {
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

boost::optional<std::string> get_env(const char *key) {
  const char *value = std::getenv(key);
  if (value) {
    return std::string(value);
  } else {
    return boost::none;
  }
}

boost::optional<std::string> get_env(std::string key) {
  return get_env(key.c_str());
}

void less(const Location &l) {
  const boost::optional<std::string> pager(get_env("GIRLPAGER"));
  std::string cmd(pager ? *pager : "less");
  cmd += " +" + std::to_string(l.get_row_number());
  cmd += " " + l.get_path().string();
  system(cmd.data());
}

std::vector<std::string> getEnvs(const char *key) {
  const boost::optional<std::string> value(get_env(key));
  std::string str(value ? *value : "");
  std::vector<std::string> vec;
  boost::algorithm::split(vec, str, boost::algorithm::is_any_of(":"));
  return vec;
}

boost::optional<Location> find_target(const boost::optional<std::string> &glossary, const std::vector<std::string> &libpaths, const std::string &target) {
  if (!glossary) {
    return find_libraries(libpaths, target);
  }
  boost::optional<boost::filesystem::path> glospath(search_libpaths(*glossary, libpaths));
  if (!glospath) {
    throw;
    // return boost::none;
  }
  return find_glossary(*glospath, target);
}

boost::program_options::variables_map parse_commandline_arguments(int argc, char **argv) {
  namespace po = boost::program_options;
  po::options_description options("commandline options");
  options.add_options()
    ("glossary,g", po::value<std::string>(), "specify glossary")
    ("query", po::value<std::vector<std::string>>(), "word to search")
  ;
  po::positional_options_description pod;
  pod.add("query", -1);
  po::variables_map vm;
  po::store(
      po::command_line_parser(argc, argv)
        .options(options)
        .positional(pod)
        .run(),
      vm);
  po::notify(vm);
  return vm;
}

template<typename T>
boost::optional<T> assoc(const std::string &option, const boost::program_options::variables_map &vm) {
  if (!vm.count(option)) {
    return boost::none;
  }
  return vm[option].as<T>();
}

std::string concat_arguments(const std::vector<std::string> &args) {
  return std::accumulate(args.begin(), args.end(), std::string(),
      [](const std::string &x, const std::string y) {
        return x.empty() ? y : x + " " + y;
      });
}

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
