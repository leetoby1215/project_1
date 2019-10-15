#include <iostream>
#include <fstream>
using namespace std;

void eliminate(bool *matrix, int matrix_rows, int matrix_cols) {
    bool *p;
    bool is_full = 1;
    int eliminate_row = 0;

    while (is_full) {
        for (int i = eliminate_row; i > 0; --i) {
            p = matrix + matrix_cols * i;
            for (int j = 0; j < matrix_cols; ++j) {
                *p = *(p - matrix_cols);
                ++p;
            }
        }
        p = matrix;
        for (int i = 0; i < matrix_rows; ++i) {
            is_full = 1;
            for (int j = 0; j < matrix_cols; ++j) {
                is_full *= *p;
                ++p;
            }
            if (is_full) {
                eliminate_row = i;
                break;
            }
        }
    }
}

void block(bool *matrix, int matrix_rows, int matrix_cols,bool *block, int block_rows, int block_cols, int col) {
    bool *p = matrix + col - 1;
    int row = block_rows - 1;

    if (col + block_cols - 1 > matrix_cols) {
        throw "Error input!";
    }

    while (row < matrix_rows) {
        bool *q = p + matrix_cols;
        bool *r = block;
        bool check = 0;
        for (int i = 0; i < block_rows && row < matrix_rows; ++i) {
            for (int j = 0; j < block_cols; ++j) {
                if (*q && *r) {
                    check = 1;
                }
                ++r;
                ++q;
            }
            q = q + matrix_cols - block_cols;
        }
        q = p;
        r = block;
        if (check || row == matrix_rows - 1) {
            for (int i = 0; i < block_rows; ++i) {
                for (int j = 0; j < block_cols; ++j) {
                    *q = (*r) ? 1 : *q;
                    ++r;
                    ++q;
                }
                q = q + matrix_cols - block_cols;
            }
            break;
        }
        p += matrix_cols;
        ++row;
    }

    
    eliminate(matrix, matrix_rows, matrix_cols);
}

int main() {
    bool T1[6] = {1, 1, 1, 0, 1, 0};
    bool T2[6] = {0, 1, 1, 1, 0, 1};
    bool T3[6] = {0, 1, 0, 1, 1, 1};
    bool T4[6] = {1, 0, 1, 1, 1, 0};
    bool L1[6] = {1, 0, 1, 0, 1, 1};
    bool L2[6] = {1, 1, 1, 1, 0, 0};
    bool L3[6] = {1, 1, 0, 1, 0, 1};
    bool L4[6] = {0, 0, 1, 1, 1, 1};
    bool J1[6] = {0, 1, 0, 1, 1, 1};
    bool J2[6] = {1, 0, 0, 1, 1, 1};
    bool J3[6] = {1, 1, 1, 0, 1, 0};
    bool J4[6] = {1, 1, 1, 0, 0, 1};
    bool S1[6] = {0, 1, 1, 1, 1, 0};
    bool S2[6] = {1, 0, 1, 1, 0, 1};
    bool Z1[6] = {1, 1, 0, 0, 1, 1};
    bool Z2[6] = {0, 1, 1, 1, 1, 0};
    bool I1[4] = {1, 1, 1, 1};
    bool I2[4] = {1, 1, 1, 1};
    bool O[4] = {1, 1, 1, 1};
    bool is_game_over = 0;
    int rows, cols;
    string s;

    ifstream fin("tetris.data");

    fin >> rows >> cols;
    if (rows > 40 || cols > 15) {
        throw "Error! The size of row or col is too large!";
    }
    if (rows < 1 || cols < 1) {
        throw "Error! The size of row or col is too small!";
    }
    rows += 4;

    bool *matrix = new bool[rows * cols];

    for (int i = 0; i < rows * cols; ++i) {
        matrix[i] = 0;
    }

    while (!is_game_over) {
        int col;
        fin >> s;
        if (s == "End") {
            break;
        }
        fin >> col;
        if (s == "T1") {
            block(matrix, rows, cols, T1, 2, 3, col);
        } else if (s == "T2") {
            block(matrix, rows, cols, T2, 3, 2, col);
        } else if (s == "T3") {
            block(matrix, rows, cols, T3, 2, 3, col);
        } else if (s == "T4") {
            block(matrix, rows, cols, T4, 3, 2, col);
        } else if (s == "L1") {
            block(matrix, rows, cols, L1, 3, 2, col);
        } else if (s == "L2") {
            block(matrix, rows, cols, L2, 2, 3, col);
        } else if (s == "L3") {
            block(matrix, rows, cols, L3, 3, 2, col);
        } else if (s == "L4") {
            block(matrix, rows, cols, L4, 2, 3, col);
        } else if (s == "J1") {
            block(matrix, rows, cols, J1, 3, 2, col);
        } else if (s == "J2") {
            block(matrix, rows, cols, J2, 2, 3, col);
        } else if (s == "J3") {
            block(matrix, rows, cols, J3, 3, 2, col);
        } else if (s == "J4") {
            block(matrix, rows, cols, J4, 2, 3, col);
        } else if (s == "S1") {
            block(matrix, rows, cols, S1, 2, 3, col);
        } else if (s == "S2") {
            block(matrix, rows, cols, S2, 3, 2, col);
        } else if (s == "Z1") {
            block(matrix, rows, cols, Z1, 2, 3, col);
        } else if (s == "Z2") {
            block(matrix, rows, cols, Z2, 3, 2, col);
        } else if (s == "I1") {
            block(matrix, rows, cols, I1, 4, 1, col);
        } else if (s == "I2") {
            block(matrix, rows, cols, I2, 1, 4, col);
        } else if (s == "O") {
            block(matrix, rows, cols, O, 2, 2, col);
        } else {
            throw "Matching blocks is Error!";
        }
        for (int i = 0; i < cols * 4; ++i) {
            if (matrix[i]) {
                is_game_over = 1;
            }
        }
    }

    fin.close();
    ofstream fout("tetris.output");

    for (int i = 4; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            fout << matrix[i * cols + j];
        }
        fout << endl;
    }
    fout.close();

    return 0;
}