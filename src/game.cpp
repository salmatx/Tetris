#include "game.h"

Game::Game(int left_move, int right_move, int rotation, int fast_drop,
           int hard_drop) {
    this->moves_.left = left_move;
    this->moves_.right = right_move;
    this->moves_.rotate = rotation;
    this->moves_.fast_drop = fast_drop;
    this->moves_.hard_drop = hard_drop;
}
