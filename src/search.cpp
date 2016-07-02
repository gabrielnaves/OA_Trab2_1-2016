#include "search.hpp"

int Search::nseeks = 0;

Registry Search::search(string data_fname, string index_fname, string key, int order) {
    Registry reg;
    ifstream data_file(data_fname), index_file(index_fname);

    if (not data_file) {
        cout << "Erro! Arquivo de dados nao encontrado!" << endl;
        return badReg();
    }

    if (not index_file) {
        cout << "Erro! Arquivo de indices em arvore-B nao encontrado!" << endl;
        return badReg();
    }

    string line;

    // Find root
    int root;
    getline(index_file, line);
    sscanf(line.c_str(), "root: %d", &root);
    index_file.seekg(10 + root*(12 + (order-1)*9 + 3 + (order-1)*4 + 3 + order*4 + 1)); // Seek to root

    nseeks = 1;
    while (true) {
        getline(index_file, line);
        auto tmp = parseLine(line, key, order);
        if (tmp.first == -1 and tmp.second == -1) {
            cout << "Erro! Registro nao encontrado." << endl;
            return badReg();
        }
        if (tmp.first == -1) {
            return retrieveFromDataFile(data_file, tmp.second);
        }
        // Seek to next page
        nseeks++;
        index_file.seekg(10 + tmp.first*(12 + (order-1)*9 + 3 + (order-1)*4 + 3 + order*4 + 1));
    }

    reg.good = true;
    return reg;
}

pair<int,int> Search::parseLine(string line, string key, int order) {
    pair<int, int> res;

    line = line.substr(6);
    int node_size;
    sscanf(line.c_str(), "%d", &node_size);
    line = line.substr(6);

    vector<string> primary_keys;
    for (int i = 0; i < node_size; ++i)
        primary_keys.push_back(msc::removeSpaces(line.substr(i*9, 9)));

    bool found_key = false;
    int i = 0;
    for (; i < node_size; ++i) {
        if (primary_keys[i] == key) {
            found_key = true;
            break;
        }
        if (primary_keys[i] > key)
            break;
    }

    if (found_key) {
        line = line.substr((order-1)*9 + 3);
        res.first = -1;
        res.second = msc::strToInt(line.substr(i*4));
    }
    else {
        line = line.substr((order-1)*9 + 3 + (order-1)*4 + 3);
        string tmp = line.substr(i*4).substr(1);
        if (tmp == "###")
            res.first = -1;
        else
            res.first = msc::strToInt(tmp);
        res.second = -1;
    }

    return res;
}

Registry Search::retrieveFromDataFile(ifstream &data_file, int offset) {
    Registry reg;
    data_file.seekg(55*offset);
    string line;
    getline(data_file, line);

    reg.name = msc::removeTrailingSpaces(line.substr(0, 40));
    reg.matr = line.substr(41, 5);
    reg.curso = line.substr(48, 2);
    reg.turma = line.substr(52, 1);

    reg.nseeks = nseeks;

    reg.good = true;
    return reg;
}

Registry Search::badReg() {
    Registry reg;
    reg.good = false;
    return reg;
}
