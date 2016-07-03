#include "gui.hpp"

bool GUI::is_ok = true;

bool GUI::ok() {
    return is_ok;
}

void GUI::run() {
    while (is_ok) {
        cout << "----------------------------------------" << endl;
        cout << "O que deseja fazer?" << endl;
        cout << "[1] Criar um arquivo de indices estruturado em arvore-B a partir de um arquivo de dados;" << endl;
        cout << "[2] Buscar, inserir ou remover um registro de um arquivo de dados;" << endl;
        cout << "[Q] Sair do programa." << endl;
        string op = ReadLine();
        if (op == "1")
            createBTreeFromDataFile();
        else if (op == "2")
            fileMenu();
        else if (op == "q" or op == "Q")
            is_ok = false;
        else
            cout << "Erro! Opcao nao existe." << endl;
    }
}

void GUI::fileMenu() {
    cout << "Digite o nome do arquivo de dados: " << endl;
    string data_fname = ReadLine();
    if (not fileExists(data_fname)) {
        cout << "Erro! Arquivo de dados nao existe." << endl;
        return;
    }

    cout << "Digite o nome do arquivo de indices: " << endl;
    string index_fname = ReadLine();

    cout << "Digite a ordem da arvore-B: " << endl;
    int order = ReadInt();

    if (not fileExists(index_fname)) {
        cout << "Aviso: O arquivo de indices nao foi encontrado. Deseja criar um agora? [S] ou [N]" << endl;
        string op = ReadLine();
        if (op == "s" or op == "S") {
            Btree btree(order, data_fname, index_fname);
            if (btree.isGood()) {
                cout << "Arvore-B criada com sucesso!" << endl;
                cout << "Visualizacao por DFS da arvore-B:" << endl;
                btree.show();
            }
            else {
                cout << "Erro na criacao do arquivo de indices." << endl;
            }
        }
        else if (op == "n" or op == "N") {
            cout << "O arquivo de indices nao foi criado." << endl;
            return;
        }
        else {
            cout << "Opcao invalida. O arquivo de indices nao foi criado." << endl;
            return;
        }
    }

    bool should_leave = false;
    while (is_ok and not should_leave) {
        cout << "----------------------------------------" << endl;
        cout << "O que deseja fazer?" << endl;
        cout << "[1] Buscar um registro;" << endl;
        cout << "[2] Inserir um registro;" << endl;
        cout << "[3] Remover um registro;" << endl;
        cout << "[Q] Voltar." << endl;
        string op = ReadLine();
        if (op == "1")
            searchReg(data_fname, index_fname, order);
        else if (op == "2")
            insertReg(data_fname, index_fname, order);
        else if (op == "3")
            cout << "Opcao ainda nao implementada." << endl;
        else if (op == "q" or op == "Q")
            should_leave = true;
        else
            cout << "Erro! Opcao nao existe." << endl;
    }
}

void GUI::createBTreeFromDataFile() {
    cout << "Digite o nome do arquivo de dados: " << endl;
    string data_fname = ReadLine();

    cout << "Digite o nome do arquivo de indices: " << endl;
    string index_fname = ReadLine();

    cout << "Digite a ordem que deseja para a arvore-B: " << endl;
    int order = ReadInt();

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

void GUI::searchReg(string data_fname, string index_fname, int order) {
    cout << "Digite o nome completo do aluno que deseja buscar: " << endl;
    string name = ReadLine();

    cout << "Digite a matricula desse aluno: " << endl;
    string matr = ReadLine();

    string primary_key = msc::makePrimaryKey(name, matr);

    Registry reg = Search::search(data_fname, index_fname, primary_key, order);
    if (reg.good) {
        cout << "---" << endl;
        cout << "Registro encontrado com sucesso!" << endl << "Dados do registro: " << endl;
        cout << "Nome completo: " << reg.name << endl;
        cout << "Matricula: " << reg.matr << endl;
        cout << "Curso: " << reg.curso << endl;
        cout << "Turma: " << reg.turma << endl;
        cout << "Numero de seeks para encontrar o registro: " << reg.nseeks << endl;
    }
}

void GUI::insertReg(string data_fname, string index_fname, int order) {
    Reg reg = make_shared<Registry>();

    cout << "Digite o nome completo do aluno que deseja inserir: " << endl;
    reg->name = ReadLine();

    cout << "Digite o numero de matricula do aluno: " << endl;
    reg->matr = ReadLine();

    cout << "Digite o curso do aluno: " << endl;
    reg->curso = ReadLine();

    cout << "Digite a turma do aluno: " << endl;
    reg->turma = ReadLine();

    reg->good = true;

    Btree btree(order, data_fname, index_fname);
    btree.insert(reg);
    if (reg->good) {
        cout << "---" << endl << "Insercao realizada com sucesso!" << endl;
        cout << "Visualizacao por DFS da arvore-B:" << endl;
        btree.show();
    }
    else
        cout << "---" << endl << "Erro! O registro ja existe!" << endl;
}

string GUI::ReadLine() {
    string line;
    getline(cin, line);
    return line;
}

int GUI::ReadInt() {
    return msc::strToInt(ReadLine());
}

bool GUI::fileExists(string fname) {
    ifstream file(fname);
    if (file)
        return true;
    return false;
}
