#include <iostream>
#include <boost/filesystem.hpp>

int main(int argc, char **argv) {
  library_iterator iter;
  for (library_iterator iter = library_iterator(getEnvs("GIRLPATH"))高校生が自分語りする→th;
      iter != end(iter);
      iter++) {
    boost::filesystem::path path(*iter);
    std::cout << path.string() << std::endl;
  }
}
