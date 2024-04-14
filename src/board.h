#pragma once

#include "common.h"
#include "piece.h"

#include <cstddef>
#include <vector>
#include <cstdint>
#include <chrono>

namespace game {

class Board {
public:
    Board();
    Board(const Board& other);
    Board& operator=(const Board& other);
    void MovePiece(const MoveTypes& move);
    uint8_t* GetPiece(PieceType type);
    int GetShadowPieceRowPosition();
    int GetPieceRowPosition(PieceType type);
    int GetPieceColumnPosition(PieceType type);
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
    void UpdateGame(const MoveTypes& input);
    GameState GetActualGamePhase() const;
    size_t GetStartLevel() const;
    size_t GetLevel() const;
    size_t GetPoints() const;

private:
    struct PieceState {
        Piece piece;
        int offset_row;
        int offset_col;
    };
    const uint8_t kFramesPerDrop[30]{
            48, 43, 38, 33, 28, 23, 18, 13, 8, 6,
            5, 5, 5, 4, 4, 4, 3, 3, 3, 2,
            2, 2, 2, 2, 2, 2, 2, 2, 2, 1
    };
    const float kTargetSecondsPerFrame = 1.0f / 60.0f;
    const size_t height_ = 22;
    const size_t width_ = 10;
    std::vector<bool> lines_to_clear_{};
    uint8_t pending_line_count_ = 0;
    size_t cleared_line_count_ = 0;
    std::vector<std::vector<uint8_t>> board_;
    std::unique_ptr<PieceState> actual_piece_ = nullptr;
    std::unique_ptr<PieceState> next_piece_ = nullptr;
    size_t points_ = 0;
    size_t level_ = 0;
    size_t start_level_ = 0;
    GameState game_phase_ = GameState::kGameStartPhase;
    float next_drop_time_ = 0;
    float time_duration_ = 0;
    float highlight_end_time_ = 0;
    std::chrono::time_point<std::chrono::steady_clock> start_time_{};
    std::chrono::time_point<std::chrono::steady_clock> current_time_{};

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
    bool SoftDrop();
    void DropPiece();
    void UpdateGameplay(const MoveTypes& input);
    void UpdateGameStart(const MoveTypes&, const size_t board_width = 10,
                         const size_t board_height = 22);
    void UpdateGameOver(const MoveTypes& input);
    void UpdateGameLines();
    float GetTimeToNextDrop();
    void SetNextGamePhase(const GameState& game_phase);
    size_t ComputePoints();
    int GetLinesForNextLevel();
    void LevelUp();
    void BoardClean();
};

}