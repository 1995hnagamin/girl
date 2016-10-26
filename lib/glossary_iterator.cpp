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

GlossaryIterator GlossaryIterator::end() const {
  return GlossaryIterator(*boost::filesystem::end(iter));
}

const boost::filesystem::path LibraryIterator::operator*() const {
  return *iter;
}

LibraryIterator &LibraryIterator::operator++() {
  iter++;
  return *this;
}

bool LibraryIterator::operator!=(const LibraryIterator &other) const {
  return iter != other.iter;
}

LibraryIterator LibraryIterator::end() const {
  return LibraryIterator(*boost::filesystem::end(iter));
}
