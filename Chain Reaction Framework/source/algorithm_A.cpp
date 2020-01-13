#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "../include/algorithm.h"
using namespace std;

#define WHITE 0
#define ME 1
#define ENEMY 2

class game;

class node {
    friend class game;
    private:
        int score;
        int color;
        int orb;
        int critical_mass;
        bool is_critical;
    public:
        node();
};

node::node() {
    score = 0;
    color = WHITE;
    orb = 0;
    critical_mass = 0;
    is_critical = false;
}

class game {
    private:
        node*** arr;
        int high_score_index[2];
    public:
        game();
        ~game();
        void get_node_property(Board board, char color);
        void evaluate_score(Board board, int x, int y);
        void check_highest_score();
        int* get_highest_score_index();
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

game::~game() {
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 6; j++) {
            delete arr[i][j];
        }
        delete[] arr[i];
    }
    delete[] arr; 
}

void game::get_node_property(Board board, char color) {
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 6; j++) {
            if (board.get_cell_color(i, j) != 'w') {
                if (board.get_cell_color(i, j) == color)
                    arr[i][j]->color = ME;
                else
                    arr[i][j]->color = ENEMY;
            }
            arr[i][j]->orb = board.get_orbs_num(i, j);
            arr[i][j]->critical_mass = board.get_capacity(i, j);
        }
    }
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 6; j++) {
            if (arr[i][j]->orb == arr[i][j]->critical_mass - 1)
                arr[i][j]->is_critical = true;
        }
    }
}

void game::evaluate_score(Board board, int x, int y) {
    int my_cell = 0;
    int enemy_cell = 0;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 6; j++) {
            if (arr[i][j]->color = ME)
                my_cell++;
            if (arr[i][j]->color = ENEMY)
                enemy_cell++;
        }
    }
    if (my_cell == 0)
        arr[x][y]->score -= 9999;
    if (enemy_cell == 0)
        arr[x][y]->score += 9999;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 6; j++) {
            if (arr[i][j]->color == ME) {
                if (i - 1 >= 0 && arr[i - 1][j]->color == ENEMY && arr[i - 1][j]->is_critical == true)
                    arr[x][y]->score -= (5 - arr[i - 1][j]->critical_mass);
                if (i + 1 <= 4 && arr[i + 1][j]->color == ENEMY && arr[i + 1][j]->is_critical == true)
                    arr[x][y]->score -= (5 - arr[i + 1][j]->critical_mass);
                if (j - 1 >= 4 && arr[i][j - 1]->color == ENEMY && arr[i][j - 1]->is_critical == true)
                    arr[x][y]->score -= (5 - arr[i][j - 1]->critical_mass);
                if (j + 1 <= 4 && arr[i][j + 1]->color == ENEMY && arr[i][j + 1]->is_critical == true)
                    arr[x][y]->score -= (5 - arr[i][j + 1]->critical_mass);
            }
        }
    }
}

void game::check_highest_score() {
    int x = 0;
    int y = 0;
    int score = -9999;

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 6; j++) {
            if (arr[i][j]->color == ME && arr[i][j]->score > score) {
                score = arr[i][j]->score;
                x = i;
                y = j;
            }
        }
    }
    high_score_index[0] = x;
    high_score_index[1] = y;
}

int* game::get_highest_score_index() {
    return high_score_index;
}

void algorithm_A(Board board, Player player, int index[]){
    srand(time(NULL));
    game simulation;
    int color = player.get_color();

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 6; j++) {
            if (board.get_cell_color(i, j) == 'w' || board.get_cell_color(i, j) == color) {
                Board new_board = board;
                new_board.place_orb(i, j, &player);
                simulation.get_node_property(new_board, color);
                simulation.evaluate_score(new_board, i, j);
                simulation.check_highest_score();
                simulation.~game();
            }
        }
    }
    index = simulation.get_highest_score_index();
}