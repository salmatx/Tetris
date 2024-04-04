#include "piece.h"
#include "board.h"

#include <iostream>

int main() {
    std::atexit(Piece::Cleanup);
    return 0;
}
