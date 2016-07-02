#ifndef GUI_HPP
#define GUI_HPP

#include "include_all_std.hpp"

#include "btree.hpp"
#include "search.hpp"

class GUI {
  public:
    static bool ok();
    static void run();

  private:

    static void createBTreeFromDataFile();
    static void searchReg();

    static bool is_ok;
};

#endif /* GUI_HPP */
