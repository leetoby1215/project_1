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
    arr = new node**[ROW];
    for (int i = 0; i < ROW; i++) {
        arr[i] = new node*[COL];
        for (int j = 0; j < COL; j++) {
            arr[i][j] = new node();
        }
    }
}

void game::get_node_property(Board board) {
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
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
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            if (arr[i][j]->orb == arr[i][j]->critical_mass - 1)
                arr[i][j]->is_critical = true;
        }
    }
}

void game::evaluate_score(Board board, int x, int y) {
    int my_cell = 0;
    int enemy_cell = 0;
    int my_critical_cell = 0;
    int my_critical_cell_alone = 0;
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
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
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            if (arr[i][j]->color == ME) {
                arr[i][j]->score++;
                if (arr[i][j]->is_critical)
                    my_critical_cell++;
                if (i - 1 >= 0 && arr[i - 1][j]->color == ENEMY && arr[i - 1][j]->is_critical)
                    arr[x][y]->score -= (5 - arr[i - 1][j]->critical_mass);
                if (i + 1 < ROW && arr[i + 1][j]->color == ENEMY && arr[i + 1][j]->is_critical)
                    arr[x][y]->score -= (5 - arr[i + 1][j]->critical_mass);
                if (j - 1 >= 0 && arr[i][j - 1]->color == ENEMY && arr[i][j - 1]->is_critical)
                    arr[x][y]->score -= (5 - arr[i][j - 1]->critical_mass);
                if (j + 1 < COL && arr[i][j + 1]->color == ENEMY && arr[i][j + 1]->is_critical)
                    arr[x][y]->score -= (5 - arr[i][j + 1]->critical_mass);
            }
        }
    }
    if (arr[0][0]->color == ME && (arr[1][0]->color != ENEMY || !arr[1][0]->is_critical) && (arr[0][1]->color != ENEMY || !arr[0][1]->is_critical)) {
        if (arr[0][0]->is_critical)
            arr[x][y]->score += 2;
        arr[x][y]->score += 3;
    }
    if (arr[ROW - 1][0]->color == ME && (arr[ROW - 2][0]->color != ENEMY || !arr[ROW - 2][0]->is_critical) && (arr[ROW - 1][1]->color != ENEMY || !arr[ROW - 1][1]->is_critical)) {
        if (arr[ROW - 1][0]->is_critical)
            arr[x][y]->score += 2;
        arr[x][y]->score += 3;
    }
    if (arr[ROW - 1][COL - 1]->color == ME && (arr[ROW - 2][COL - 1]->color != ENEMY || !arr[ROW - 2][COL - 1]->is_critical) && (arr[ROW - 1][COL - 2]->color != ENEMY || !arr[ROW - 1][COL - 2]->is_critical)) {
        if (arr[ROW - 1][COL - 1]->is_critical)
            arr[x][y]->score += 2;
        arr[x][y]->score += 3;
    }
    if (arr[0][COL - 1]->color == ME && (arr[1][COL - 1]->color != ENEMY || !arr[1][COL - 1]->is_critical) && (arr[0][COL - 2]->color != ENEMY || !arr[0][COL - 2]->is_critical)) {
        if (arr[0][COL - 1]->is_critical)
            arr[x][y]->score += 2;
        arr[x][y]->score += 3;
    }
    for (int i = 1; i <= ROW - 2; i++) {
        if (arr[i][0]->color == ME && (arr[i - 1][0]->color != ENEMY || !arr[i - 1][0]->is_critical) && (arr[i][1]->color != ENEMY || !arr[i][1]->is_critical) && (arr[i + 1][0]->color != ENEMY || !arr[i + 1][0]->is_critical)) {
            if (arr[i][0]->is_critical)
                arr[x][y]->score += 2;
            arr[x][y]->score += 2;
        }
        if (arr[i][COL - 1]->color == ME && (arr[i - 1][COL - 1]->color != ENEMY || !arr[i - 1][COL - 1]->is_critical) && (arr[i][COL - 2]->color != ENEMY || !arr[i][COL - 2]->is_critical) && (arr[i + 1][COL - 1]->color != ENEMY || !arr[i + 1][COL - 1]->is_critical)) {
            if (arr[i][COL - 1]->is_critical)
                arr[x][y]->score += 2;
            arr[x][y]->score += 2;
        }
    }
    for (int i = 1; i <= COL - 2; i++) {
        if (arr[0][i]->color == ME && (arr[0][i - 1]->color != ENEMY || !arr[0][i - 1]->is_critical) && (arr[1][i]->color != ENEMY || !arr[1][i]->is_critical) && (arr[0][i + 1]->color != ENEMY || !arr[0][i + 1]->is_critical)) {
            if (arr[0][i]->is_critical)
                arr[x][y]->score += 2;
            arr[x][y]->score += 2;
        }
        if (arr[ROW - 1][i]->color == ME && (arr[ROW - 1][i - 1]->color != ENEMY || !arr[ROW - 1][i - 1]->is_critical) && (arr[ROW - 2][i]->color != ENEMY || !arr[ROW - 2][i]->is_critical) && (arr[ROW - 1][i + 1]->color != ENEMY || !arr[ROW - 1][i + 1]->is_critical)) {
            if (arr[ROW - 1][i]->is_critical)
                arr[x][y]->score += 2;
            arr[x][y]->score += 2;
        }
    }
    for (int i = 1; i <= ROW - 2; i++) {
        for (int j = 1; j <= COL - 2; j++) {
            if (arr[i][j]->color == ME && (arr[i - 1][j]->color != ENEMY || !arr[i - 1][j]->is_critical) && (arr[i + 1][j]->color != ENEMY || !arr[i + 1][j]->is_critical) && (arr[i][j - 1]->color != ENEMY || !arr[i][j - 1]->is_critical) && (arr[i][j + 1]->color != ENEMY || !arr[i][j + 1]->is_critical))
                if (arr[i][j]->is_critical)
                    arr[x][y]->score += 2;
        }
    }
    if (arr[0][0]->color == ME && (arr[1][0]->color != ME || !arr[1][0]->is_critical) && (arr[0][1]->color != ME || !arr[0][1]->is_critical))
        my_critical_cell_alone++;
    if (arr[ROW - 1][0]->color == ME && (arr[ROW - 2][0]->color != ME || !arr[ROW - 2][0]->is_critical) && (arr[ROW - 1][1]->color != ME || !arr[ROW - 1][1]->is_critical))
        my_critical_cell_alone++;
    if (arr[ROW - 1][COL - 1]->color == ME && (arr[ROW - 2][COL - 1]->color != ME || !arr[ROW - 2][COL - 1]->is_critical) && (arr[ROW - 1][COL - 2]->color != ME || !arr[ROW - 1][COL - 2]->is_critical))
        my_critical_cell_alone++;
    if (arr[0][COL - 1]->color == ME && (arr[1][COL - 1]->color != ME || !arr[1][COL - 1]->is_critical) && (arr[0][COL - 2]->color != ME || !arr[0][COL - 2]->is_critical))
        my_critical_cell_alone++;
    for (int i = 1; i <= ROW - 2; i++) {
        if (arr[i][0]->color == ME && (arr[i - 1][0]->color != ME || !arr[i - 1][0]->is_critical) && (arr[i][1]->color != ME || !arr[i][1]->is_critical) && (arr[i + 1][0]->color != ME || !arr[i + 1][0]->is_critical))
            my_critical_cell_alone++;
        if (arr[i][COL - 1]->color == ME && (arr[i - 1][COL - 1]->color != ME || !arr[i - 1][COL - 1]->is_critical) && (arr[i][COL - 2]->color != ME || !arr[i][COL - 2]->is_critical) && (arr[i + 1][COL - 1]->color != ME || !arr[i + 1][COL - 1]->is_critical))
            my_critical_cell_alone++;
    }
    for (int i = 1; i <= COL - 2; i++) {
        if (arr[0][i]->color == ME && (arr[0][i - 1]->color != ME || !arr[0][i - 1]->is_critical) && (arr[1][i]->color != ME || !arr[1][i]->is_critical) && (arr[0][i + 1]->color != ME || !arr[0][i + 1]->is_critical))
            my_critical_cell_alone++;
        if (arr[ROW - 1][i]->color == ME && (arr[ROW - 1][i - 1]->color != ME || !arr[ROW - 1][i - 1]->is_critical) && (arr[ROW - 2][i]->color != ME || !arr[ROW - 2][i]->is_critical) && (arr[ROW - 1][i + 1]->color != ME || !arr[ROW - 1][i + 1]->is_critical))
            my_critical_cell_alone++;
    }
    arr[x][y]->score += 2 * (my_critical_cell - my_critical_cell_alone);
}

void game::check_highest_score() {
    int score = -9999;

    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            if (arr[i][j]->color != ENEMY && arr[i][j]->score > score) {
                score = arr[i][j]->score;
                high_score_index_x = i;
                high_score_index_y = j;
            }
        }
    }
}

void game::reset() {
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
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
    game simulation(&player);
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
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
}