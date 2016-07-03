#ifndef REGISTRY_HPP
#define REGISTRY_HPP

#include "include_all_std.hpp"

struct Registry {
    string name;
    string matr;
    string curso;
    string turma;

    int nseeks;

    bool good;
};

typedef shared_ptr<Registry> Reg;

#endif /* REGISTRY_HPP */
