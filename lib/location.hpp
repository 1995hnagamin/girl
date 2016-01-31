#ifndef GIRL_SRC_LOCATION_HPP
#define GIRL_SRC_LOCATION_HPP

#include <boost/filesystem.hpp>
#include <string>

class Location {
  private:
    boost::filesystem::path path;
    int row_number;
  public:
    Location(std::string p, int r) :
      path(p), row_number(r) {;}
    boost::filesystem::path get_path();
    int get_row_number();
};

#endif
