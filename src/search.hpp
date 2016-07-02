#ifndef SEARCH_HPP
#define SEARCH_HPP

#include "include_all_std.hpp"

#include "registry.hpp"
#include "msc_operations.hpp"

class Search {
  public:
    static Registry search(string data_fname, string index_fname, string key);

  private:
    static pair<int,int> parseLine(string line, string key);
    static Registry retrieveFromDataFile(ifstream& data_file, int offset);
    static Registry badReg();

    static int nseeks;
};

#endif /* SEARCH_HPP */
