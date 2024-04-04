#include "board.h"

#include <iostream>

//#define NDEBUG //uncomment in release to disable assert()

int main() {
    std::atexit(game::Piece::Cleanup);
    return 0;
}
