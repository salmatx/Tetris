#include "piece.h"
#include <iostream>

int main() {
    Piece piece{kSquare};
    Piece::MakeAllRotations();
    Piece::QuitGame();
    return 0;
}
