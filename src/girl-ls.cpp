#include "hogehoge.hpp"
#include <iostream>
#include <boost/filesystem.hpp>
#include <vector>
#include <string>

int main(int argc, char **argv) {
  std::vector<std::string> girlpaths(getEnvs("GIRLPATH"));
  for (std::string girlpath : girlpaths) {
    for (auto iter = boost::filesystem::directory_iterator(girlpath);
        iter != boost::filesystem::end(iter);
        ++iter) {
      std::cout << *iter << std::endl;
    }
  }
}
