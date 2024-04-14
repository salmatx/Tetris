#pragma once

#include "common.h"
#include "piece.h"

#include <cstddef>
#include <vector>
#include <cstdint>

namespace game {

class Board {
public:
    Board(size_t width, size_t height);
    void MovePiece(const MoveTypes& move);
    bool SoftDrop();
    uint8_t* GetPiece(PieceType type);
    int GetShadowPieceRowPosition();
    int GetRowPosition(PieceType type);
    int GetColumnPosition(PieceType type);
    uint16_t GetPieceSize(PieceType type);
    size_t GetBoardHeight();
    size_t GetBoardWidth();
    std::vector<std::vector<uint8_t>> GetBoard();
    int FindLinesToClear();
    void ClearLines();
    void SetPendingLineCount(uint8_t value);
    uint8_t GetPendingLineCount();
    void SetClearedLineCount(uint8_t value);
    size_t GetClearedLineCount() const;
    bool IsLineClearing(int index) const;
    bool CheckRowEmpty(const int& row) const;

private:
    struct PieceState {
        Piece piece;
        int offset_row;
        int offset_col;
    };
    const size_t height_;
    const size_t width_;
    std::vector<bool> lines_to_clear_;
    uint8_t pending_line_count_ = 0;
    size_t cleared_line_count_ = 0;
    std::vector<std::vector<uint8_t>> board_;
    std::unique_ptr<PieceState> actual_piece_ = nullptr;
    std::unique_ptr<PieceState> next_piece_ = nullptr;

    void SetValue(const int& row, const int& col, const uint8_t value);
    uint8_t GetValue(const int& row, const int& col) const;
    bool CheckPieceValid(const PieceState& piece);
    void MovePieceLeft();
    void MovePieceRight();
    void RotatePiece();
    void MergePieceIntoBoard();
    void MakePiece(int offset_row, int offset_col);
    Shape SelectRandomPiece();
    void HardDrop();
    bool CheckRowFilled(const int& row) const;
};

}