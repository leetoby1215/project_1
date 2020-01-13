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
        char color;
        int high_score_index_x;
        int high_score_index_y;
    public:
        game(Player* player);
        void get_node_property(Board board);
        void evaluate_score(Board board, int x, int y);
        void check_highest_score();
        void reset();
        int get_highest_score_index_x();
        int get_highest_score_index_y();
};

game::game(Player* player) {
    color = player->get_color();
    high_score_index_x = 0;
    high_score_index_y = 0;
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
            if (board.get_cell_color(i, j) == 'w')
                arr[i][j]->color = WHITE;
            else if (board.get_cell_color(i, j) == color)
                arr[i][j]->color = ME;
            else
                arr[i][j]->color = ENEMY;
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
            if (arr[i][j]->color == ME)
                my_cell++;
            if (arr[i][j]->color == ENEMY)
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
                if (i + 1 < 5 && arr[i + 1][j]->color == ENEMY && arr[i + 1][j]->is_critical == true)
                    arr[x][y]->score -= (5 - arr[i + 1][j]->critical_mass);
                if (j - 1 >= 0 && arr[i][j - 1]->color == ENEMY && arr[i][j - 1]->is_critical == true)
                    arr[x][y]->score -= (5 - arr[i][j - 1]->critical_mass);
                if (j + 1 < 6 && arr[i][j + 1]->color == ENEMY && arr[i][j + 1]->is_critical == true)
                    arr[x][y]->score -= (5 - arr[i][j + 1]->critical_mass);
            }
        }
    }
}

void game::check_highest_score() {
    int score = -9999;

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 6; j++) {
            cout << arr[i][j]->color << ' ' << arr[i][j]->score << endl;
            if (arr[i][j]->color != ENEMY && arr[i][j]->score > score) {
                score = arr[i][j]->score;
                high_score_index_x = i;
                high_score_index_y = j;
            }
        }
    }
    cout << score << ' ';
}

void game::reset() {
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 6; j++) {
            arr[i][j]->score = 0;
        }
    }
}

int game::get_highest_score_index_x() {
    return high_score_index_x;
}

int game::get_highest_score_index_y() {
    return high_score_index_y;
}

void algorithm_A(Board board, Player player, int index[]){
    srand(time(NULL));
    game simulation(&player);
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 6; j++) {
            if (board.get_cell_color(i, j) == 'w' || board.get_cell_color(i, j) == player.get_color()) {
                Board new_board = board;
                new_board.place_orb(i, j, &player);
                simulation.get_node_property(new_board);
                simulation.evaluate_score(new_board, i, j);
            }
        }
    }
    simulation.check_highest_score();
    simulation.reset();
    index[0] = simulation.get_highest_score_index_x();
    index[1] = simulation.get_highest_score_index_y();
    cout << index[0] << ' ' << index[1];
    system("pause");
}