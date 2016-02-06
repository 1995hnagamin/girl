#include "hogehoge.hpp"
#include <fstream>
#include <string>
#include <boost/algorithm/string.hpp>
#include <cstdlib>
#include <wordexp.h>

const int RUNTIME_ERROR = 1;

boost::optional<Location> find_section(const boost::filesystem::path &filepath, const std::string &target) {
  std::ifstream ifs(filepath.string().c_str());

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
  throw RUNTIME_ERROR;
}

void less(const Location &l) {
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

boost::optional<Location> find_target(const boost::optional<std::string> &glossary, const std::vector<std::string> &libpaths, const std::string &target) {
  if (!glossary) {
    return find_libraries(libpaths, target);
  }
  boost::optional<boost::filesystem::path> glospath(search_libpaths(*glossary, libpaths));
  if (!glospath) {
    throw RUNTIME_ERROR;
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

std::string concat_arguments(const std::vector<std::string> &args) {
  return std::accumulate(args.begin(), args.end(), std::string(),
      [](const std::string &x, const std::string y) {
        return x.empty() ? y : x + " " + y;
      });
}


