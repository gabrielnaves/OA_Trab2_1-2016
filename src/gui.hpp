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

    static void fileMenu();

    static void createBTreeFromDataFile();
    static void searchReg(string data_fname, string index_fname, int order);

    static bool is_ok;

    static string ReadLine();
    static int ReadInt();

    static bool fileExists(string fname);
};

#endif /* GUI_HPP */
