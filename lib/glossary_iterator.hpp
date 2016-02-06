#include <boost/filesystem.hpp>
#include <vector>
#include <string>

class GlossaryIterator {
  private:
    boost::filesystem::recursive_directory_iterator iter;
  public:
    GlossaryIterator(boost::filesystem::path g) :
      iter(boost::filesystem::recursive_directory_iterator(g)) {;}
    const boost::filesystem::path operator*() const;
    GlossaryIterator &operator++();
    bool operator!=(const GlossaryIterator &other) const;
    boost::filesystem::recursive_directory_iterator end() const;
};

class LibraryIterator {
  private:
    boost::filesystem::path library;
  public:
    LibraryIterator(boost::filesystem::path l) : library(l) {;}
    const boost::filesystem::path operator*() const;
    LibraryIterator &operator++();
    bool operator!=(const LibraryIterator &other) const;
};
