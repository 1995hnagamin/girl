#include "location.hpp"

boost::filesystem::path Location::get_path() {
  return path;
}

int Location::get_row_number() {
  return row_number;
}
