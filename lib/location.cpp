#include "location.hpp"

Location::Location(
    const boost::filesystem::path& p,
    int r) :
  path(p),
  row_number(r) {}

const boost::filesystem::path& Location::get_path() const {
  return path;
}

int Location::get_row_number() const {
  return row_number;
}
