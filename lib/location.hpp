#ifndef GIRL_SRC_LOCATION_HPP
#define GIRL_SRC_LOCATION_HPP

#include <boost/filesystem.hpp>
#include <string>

class Location {
  public:
    Location(
        const boost::filesystem::path& p,
        int r);
    const boost::filesystem::path& get_path() const;
    int get_row_number() const;
  private:
    boost::filesystem::path path;
    int row_number;
};

#endif
