#pragma once

#include "piece.h"

#include <cstddef>
#include <vector>
#include <cstdint>

class Board {
public:
    Board();
    Board(size_t width, size_t height);

private:
    struct PieceState {
        Piece piece;
        int offset_row;
        int offset_col;
    };
    const size_t height_ = 22;
    const size_t width_ = 10;
    std::vector<std::vector<uint8_t>> board_;

    void SetValue(const int& row, const int& col, const uint8_t value);
    uint8_t GetValue(const int& row, const int& col);
    bool CheckPieceValid(const PieceState& piece);
};