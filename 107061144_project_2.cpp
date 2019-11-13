#include <iostream>
using namespace std;

class graph;
class queue;

class node {
friend class graph;
friend class queue;
private:
    int vertex[2];
    node* next;
public:
    node(int *des): next(NULL) {
        vertex[0] = des[0];
        vertex[1] = des[1];
    }
};

class queue {
private:
    node* Front;
    node* Back;
public:
    queue(): Front(NULL), Back(NULL) {}
    void push(int* ver) {
        node* newNode = new node(ver);
        if (!Front && !Back) {
            Front = newNode;
            Back = newNode;
        } else {
            Back->next = newNode;
            Back = newNode;
        }
    }
    void pop() {
        if (!Front && !Back) {
            cout << "The queue is empty!" << endl;
            exit(-1);
        } else {
            node* tmp = Front;
            Front = Front->next;
            delete tmp;
            tmp = NULL;
            if (!Front) {
                Back = NULL;
            }
        }
    }
    bool empty() {
        return !(Front && Back);
    }
    int* front() {
        return Front->vertex;
    }
    int* back() {
        return Back->vertex;
    }
};

class graph {
private:
    node*** lists;
    int*** predecessor;
    bool** finish;
    char** map;
    int** distance;
    int row;
    int col;
    int battery;
    int clean_num;
    int cleaned_num;
    int longest_distance;
    int steps;
    int start[2];
public:
    graph(char** map, int row, int col, int battery):
    map(map), row(row), col(col), battery(battery), clean_num(0), cleaned_num(0), longest_distance(0), steps(0) {
        lists = new node**[row];
        predecessor = new int**[row];
        distance = new int*[row];
        finish = new bool*[row];
        for (int i = 0; i < row; ++i) {
            lists[i] = new node*[col];
            predecessor[i] = new int*[col];
            distance[i] = new int[col];
            finish[i] = new bool[col];
            for (int j = 0; j < col; ++j) {
                lists[i][j] = NULL;
                predecessor[i][j] = new int[2];
                predecessor[i][j][0] = -1;
                predecessor[i][j][1] = -1;
                distance[i][j] = -1;
                if (map[i][j] == '1') {
                    finish[i][j] = true;
                } else {
                    finish[i][j] = false;
                }
                if (map[i][j] != '1') {
                    int src[2] = {i, j};
                    ++clean_num;
                    if (i + 1 < row && map[i + 1][j] != '1') {
                        int des[2] = {i + 1, j};
                        addEdge(src, des);
                    }
                    if (i - 1 >= 0 && map[i - 1][j] != '1') {
                        int des[2] = {i - 1, j};
                        addEdge(src, des);
                    }
                    if (j + 1 < col && map[i][j + 1] != '1') {
                        int des[2] = {i, j + 1};
                        addEdge(src, des);
                    }
                    if (j - 1 >= 0 && map[i][j - 1] != '1') {
                        int des[2] = {i, j - 1};
                        addEdge(src, des);
                    }
                }
                if (map[i][j] == 'R') {
                    start[0] = i;
                    start[1] = j;
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
    void BFS_check() {
        BFS_check(start);
        if (longest_distance * 2 > battery) {
            cout << "Lack of power!" << endl;
            exit(-1);
        }
    }
    void BFS_check(int* src) {
        bool** visited;
        int* vertex;
        node* current;
        queue q;

        q.push(src);
        visited = new bool*[row];
        for (int i = 0; i < row; ++i) {
            visited[i] = new bool[col];
            for (int j = 0; j < col; ++j) {
                visited[i][j] = false;
            }
        }
        visited[src[0]][src[1]] = true;
        distance[src[0]][src[1]] = 0;
        while (!q.empty()) {
            int w[2];
            src = q.front();
            w[0] = src[0];
            w[1] = src[1];
            current = lists[w[0]][w[1]];
            q.pop();
            while (current) {
                vertex = current->vertex;
                if (!visited[vertex[0]][vertex[1]]) {
                    q.push(vertex);
                    visited[vertex[0]][vertex[1]] = true;
                    predecessor[vertex[0]][vertex[1]][0] = w[0];
                    predecessor[vertex[0]][vertex[1]][1] = w[1];
                    distance[vertex[0]][vertex[1]] = distance[w[0]][w[1]] + 1;
                }
                current = current->next;
            }
        }
        for (int i = 0; i < row; ++i) {
            for (int j = 0; j < col; ++j) {
                if (map[i][j] != '1' && !visited[i][j]) {
                    cout << "Exist unreachable free cells!" << endl;
                    exit(-1);
                }
                if (longest_distance < distance[i][j])
                    longest_distance = distance[i][j];
            }
        }
        delete visited;
        visited = NULL;
    }
    void cleaning() {
        int farthest[2];
        int w[2];
        int dist;
        int charge;
        int m, n;

        finish[start[0]][start[1]] = true;
        ++cleaned_num;
        cout << start[0] << ' ' << start[1] << endl;
        while (cleaned_num < clean_num) {
            dist = 0;
            for (int i = 0; i < row; ++i) {
                for (int j = 0; j < col; ++j) {
                    if (!finish[i][j] && distance[i][j] > dist) {
                        dist = distance[i][j];
                        farthest[0] = i;
                        farthest[1] = j;
                        m = i;
                        n = j;
                    }
                }
            }
            charge = battery;
            int** path = new int*[dist];
            for (int i = 0; i < dist; ++i) {
                path[i] = new int[2];
            }
            for (int i = dist - 1; i >= 0; --i) {
                path[i][0] = farthest[0];
                path[i][1] = farthest[1];
                w[0] = farthest[0];
                w[1] = farthest[1];
                farthest[0] = predecessor[w[0]][w[1]][0];
                farthest[1] = predecessor[w[0]][w[1]][1];
            }
            for (int i = 0; i < dist; ++i) {
                cout << path[i][0] << ' ' << path[i][1] << endl;
            }
            for (int i = 0; i < dist; ++i) {
                if (!finish[path[i][0]][path[i][1]]) {
                    finish[path[i][0]][path[i][1]] = true;
                    ++cleaned_num;
                }
                --charge;
                ++steps;
            }
            while (charge >= distance[m][n]) {
                if (!finish[m + 1][n] && charge - 1 >= distance[m + 1][n])
                    m = m + 1;
                else if (!finish[m - 1][n] && charge - 1 >= distance[m - 1][n])
                    m = m - 1;
                else if (!finish[m][n + 1] && charge - 1 >= distance[m][n + 1])
                    n = n + 1;
                else if (!finish[m][n - 1] && charge - 1 >= distance[m][n - 1])
                    n = n - 1;
                else
                    break;
                finish[m][n] = true;
                ++cleaned_num;
                ++steps;
                --charge;
                cout << m << ' ' << n << endl;
            }
            dist = distance[m][n];
            for (int i = 0; i < dist; ++i) {
                w[0] = predecessor[m][n][0];
                w[1] = predecessor[m][n][1];
                m = w[0];
                n = w[1];
                if (!finish[m][n]) {
                    finish[m][n] = true;
                    ++cleaned_num;
                }
                ++steps;
                cout << m << ' ' << n << endl;
            }
            delete path;
        }
    }
    void print() {
        cout << steps << endl;
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
    if (row > 1000 || col > 1000) {
        cout << "Invalid size!";
        exit(-1);
    }
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
    
    graph g(map, row, col, battery);
    g.BFS_check();
    g.cleaning();
    g.print();
    return 0;
}