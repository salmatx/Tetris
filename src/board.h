#pragma once

#include "common.h"
#include "piece.h"

#include <cstddef>
#include <vector>
#include <cstdint>

namespace game {

class Board {
public:
    Board();
    Board(size_t width, size_t height);
    void MovePiece(const MoveTypes& move);
    bool SoftDrop();
    uint8_t* GetPiece();
    int GetRowPosition();
    int GetColumnPosition();
    uint16_t GetPieceSize();
    size_t GetBoardHeight();
    size_t GetBoardWidth();
    std::vector<std::vector<uint8_t>> GetBoard();
    int FindLinesToClear();

private:
    struct PieceState {
        Piece piece;
        int offset_row;
        int offset_col;
    };
    const size_t height_ = 22;
    const size_t width_ = 10;
    std::vector<bool> lines_to_clear_;
    std::vector<std::vector<uint8_t>> board_;
    std::unique_ptr<PieceState> actual_piece_ = nullptr;

    void SetValue(const int& row, const int& col, const uint8_t value);
    uint8_t GetValue(const int& row, const int& col) const;
    bool CheckPieceValid(const PieceState& piece);
    void MovePieceLeft();
    void MovePieceRight();
    void RotatePiece();
    void MergePieceIntoBoard();
    void MakePiece(Shape shape, int offset_row, int offset_col);
    Shape SelectRandomPiece();
    void HardDrop();
    bool CheckRowFilled(const int& row) const;
    void ClearLines();
};

}