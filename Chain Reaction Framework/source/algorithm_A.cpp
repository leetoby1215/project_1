#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "../include/algorithm.h"
using namespace std;

#define w 0
#define r 1
#define b 2
/******************************************************
 * In your algorithm, you can just use the the funcitons
 * listed by TA to get the board information.(functions 
 * 1. ~ 4. are listed in next block)
 * 
 * The STL library functions is not allowed to use.
******************************************************/

/*************************************************************************
 * 1. int board.get_orbs_num(int row_index, int col_index)
 * 2. int board.get_capacity(int row_index, int col_index)
 * 3. char board.get_cell_color(int row_index, int col_index)
 * 4. void board.print_current_board(int row_index, int col_index, int round)
 * 
 * 1. The function that return the number of orbs in cell(row, col)
 * 2. The function that return the orb capacity of the cell(row, col)
 * 3. The function that return the color fo the cell(row, col)
 * 4. The function that print out the current board statement
*************************************************************************/
class game;

class node {
    friend class game;
    private:
        int score;
        int color;
        int orb;
        bool is_critical;
    public:
        node();
};

node::node() {
    score = 0;
    color = w;
    orb = 0;
    is_critical = 0;
}

class game {
    private:
        node*** arr;
        int high_score_index[2];
    public:
        game();
        void get_node_property(Board board);
        void evaluate_score();
        void check_highest_score();
};

game::game() {
    arr = new node**[5];
    for (int i = 0; i < 5; i++) {
        arr[i] = new node*[6];
        for (int j = 0; j < 6; j++) {
            arr[i][j] = new node();
        }
    }
}

void game::get_node_property(Board board) {
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 6; j++) {
            arr[i][j]->color = board.get_cell_color(i, j);
            arr[i][j]->orb = board.get_orbs_num(i, j);
        }
    }
}

void algorithm_A(Board board, Player player, int index[]){
    srand(time(NULL));
    int row, col;
    int color = player.get_color();


    index[0] = row;
    index[1] = col;
}