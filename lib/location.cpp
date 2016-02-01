#include "location.hpp"

boost::filesystem::path Location::get_path() const {
  return path;
}

int Location::get_row_number() const {
  return row_number;
}
