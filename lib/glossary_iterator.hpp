#include <boost/filesystem.hpp>
#include <vector>
#include <tuple>

template<typename Iterator>
class MultipleIterator {
  using T = tuple<Iterator, Iterator>;
  private:
    const std::vector<T> iters;
    size_t index;
  public:
    MultipleIterator(const std::vector<T> &i) : iters(i) {;}

};

class GlossaryIterator {
  private:
    boost::filesystem::recursive_directory_iterator iter;
  public:
    GlossaryIterator(boost::filesystem::path g) :
      iter(boost::filesystem::recursive_directory_iterator(g)) {;}
    const boost::filesystem::path operator*() const;
    GlossaryIterator &operator++();
    bool operator!=(const GlossaryIterator &other) const;
    GlossaryIterator end() const;
};

class LibraryIterator {
  private:
    boost::filesystem::directory_iterator dir;
  public:
    LibraryIterator(boost::filesystem::path l) : library(l) {;}
    const boost::filesystem::path operator*() const;
    LibraryIterator &operator++();
    bool operator!=(const LibraryIterator &other) const;
    LibraryIterator end() const;
};
