#include "gui.hpp"

bool GUI::is_ok = true;

bool GUI::ok() {
    return is_ok;
}

void GUI::run() {
    cout << "----------------------------------------" << endl;
    cout << "O que deseja fazer?" << endl;
    cout << "[1] Criar um arquivo de indices estruturado em arvore-B a partir de um arquivo de dados;" << endl;
    cout << "[2] Buscar um registro em um arquivo de dados utilizando o seu arquivo de indice em arvore-B;" << endl;
    cout << "[Q] Sair do programa." << endl;
    string op;
    getline(cin, op);
    if (op == "1")
        createBTreeFromDataFile();
    else if (op == "2")
        searchReg();
    else if (op == "q" or op == "Q")
        is_ok = false;
    else
        cout << "Erro! Opcao nao existe." << endl;
}

void GUI::createBTreeFromDataFile() {
    cout << "Digite o nome do arquivo de dados: " << endl;
    string data_fname;
    cin >> data_fname;

    cout << "Digite o nome do arquivo de indices: " << endl;
    string index_fname;
    cin >> index_fname;

    cout << "Digite a ordem que deseja para a arvore-B: " << endl;
    int order;
    cin >> order;

    fstream index_file(index_fname);
    if (index_file) {
        index_file.close();
        remove(index_fname.c_str());
    }

    Btree btree(order, data_fname, index_fname);
    if (btree.isGood()) {
        cout << "Arvore-B criada com sucesso!" << endl;
        cout << "Visualizacao por DFS da arvore-B:" << endl;
        btree.show();
    }
}

void GUI::searchReg() {
    // TODO
}
