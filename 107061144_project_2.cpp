#include <iostream>
using namespace std;

class graph;

class node {
friend class graph;
private:
    int *vertex;
    node* next;
public:
    node(int *vertex): vertex(vertex), next(NULL) {}
};

class graph {
private:
    int row;
    int col;
    int chargeVertex[2];
    node*** lists;
public:
    graph(char** map, int col, int rol): row(row), col(col), lists(NULL) {
        lists = new node**[row];
        for (int i = 0; i < row; ++i) {
            lists[i] = new node*[col];
            for (int j = 0; j < col; ++j) {
                lists[i][j] = NULL;
                if (map[i][j] != '1') {
                    int src[2] = {i, j};s
                    if (map[i + 1][j] == '0' || map[i + 1][j] == 'R') {
                        int des[2] = {i + 1, j};
                        addEdge(src, des);
                    }
                    if (map[i - 1][j] == '0' || map[i - 1][j] == 'R') {
                        int des[2] = {i - 1, j};
                        addEdge(src, des);
                    }
                    if (map[i][j + 1] == '0' || map[i][j + 1] == 'R') {
                        int des[2] = {i, j + 1};
                        addEdge(src, des);
                    }
                    if (map[i][j - 1] == '0' || map[i][j - 1] == 'R') {
                        int des[2] = {i, j - 1};
                        addEdge(src, des);
                    }
                }
            }
        }
    }
    void addEdge(int *src, int *des) {
        node* current = lists[src[0]][src[1]];
        node* newNode = new node(des);
        if (!current) {
            lists[src[0]][src[1]] = newNode;
            return;
        }
        while (current->next) {
            current = current->next;
        }
        current->next = newNode;
    }
    void print() {
        node* current;
        for (int i = 0; i < row; ++i) {
            for (int j = 0; j < col; ++j) {
                if (lists[i][j]) {
                    current = lists[i][j];
                    while (current) {
                        cout << '(' << i << ", " << j << ')' << "---->" << '(' << current->vertex[0] << ", " << current->vertex[1] << ')' << endl;
                        current = current->next;
                    }
                }
            }
        }
    }
};

void check_test_case(char **map, int row, int col, int battery) {
    int number_of_R = 0;

    if (battery > 2147483647) {
        cout << "Valid capacity of battery!";
        exit(-1);
    }
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            if (map[i][j] != '1' && map[i][j] != '0' && map[i][j] != 'R') {
                cout << "Exist invalid parameter!";
                exit(-1);
            }
            if (i == 0 || i == row - 1 || j == 0 || j == col - 1) {
                if (map[i][j] == '0') {
                    cout << "Exist invalid parameter!";
                    exit(-1);
                }
            } else if (map[i][j] == 'R') {
                cout << "The location of R must be at the walls!";
                exit(-1);
            }
            if (map[i][j] == 'R')
                ++number_of_R;
        }
    }
    if (number_of_R != 1) {
        cout << "Valid number of R!";
        exit(-1);
    }
}

int main() {
    int row, col;
    int battery;
    char **map;

    cin >> row >> col >> battery;
    if (row > 1000 || col > 1000)
        throw "Invalid size!";
    map = new char*[row];
    for (int i = 0; i < row; ++i) {
        char input;
        map[i] = new char[col];
        for (int j = 0; j < col; ++j) {
            cin >> input;
            map[i][j] = input;
        }
    }
    check_test_case(map, row, col, battery);
    
    graph g(map, row, col);
    g.print();
    return 0;
}