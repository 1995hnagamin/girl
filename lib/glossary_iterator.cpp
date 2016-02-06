#include "glossary_iterator.hpp"
#include <boost/filesystem.hpp>

const boost::filesystem::path GlossaryIterator::operator*() const {
  return *iter;
}

GlossaryIterator &GlossaryIterator::operator++() {
  iter++;
  return *this;
}

bool GlossaryIterator::operator!=(const GlossaryIterator &other) const {
  return iter != other.iter;
}

boost::filesystem::recursive_directory_iterator GlossaryIterator::end() const {
  return boost::filesystem::end(iter);
}
