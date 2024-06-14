#pragma once

#include "i_board.h"

#include <cstdint>
#include <chrono>

namespace game {

class Board : public IBoard{
public:
    Board();
    Board(const Board& other);
    Board& operator=(const Board& other);
    std::shared_ptr<tetrino[]> GetPiece(const PieceType type) const override;
    int GetShadowPieceRowPosition() override;
    int GetPieceRowPosition(const PieceType type) const override;
    int GetPieceColumnPosition(const PieceType type) const override;
    uint16_t GetPieceSize(const PieceType type) const override;
    uint8_t GetBoardHeight() const override;
    uint8_t GetBoardWidth() const override;
    std::vector<std::vector<uint8_t>> GetBoard() const override;
    size_t GetClearedLineCount() const override;
    bool IsLineClearing(int index) const override;
    void UpdateGame(MoveTypes input) override;
    GameState GetActualGamePhase() const override;
    size_t GetStartLevel() const override;
    size_t GetLevel() const override;
    size_t GetPoints() const override;

private:
    struct PieceState {
        std::shared_ptr<Piece> piece;
        int offset_row;
        int offset_col;
    };
    const uint8_t kFramesPerDrop[30]{
            48, 43, 38, 33, 28, 23, 18, 13, 8, 6,
            5, 5, 5, 4, 4, 4, 3, 3, 3, 2,
            2, 2, 2, 2, 2, 2, 2, 2, 2, 1
    };
    const float kTargetSecondsPerFrame = 1.0f / 60.0f;
    const uint8_t height_ = 22;
    const uint8_t width_ = 10;
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

    void UpdateGameplay(const MoveTypes input);
    void UpdateGameStart(const MoveTypes);
    void UpdateGameOver(const MoveTypes input);
    void UpdateGameLines();
    void MovePiece(const MoveTypes move);
    void MovePieceLeft();
    void MovePieceRight();
    void RotatePiece();
    void HardDrop();
    void SetNextDrop();
    bool SoftDrop();
    float GetTimeToNextDrop();
    bool CheckRowFilled(const int& row) const;
    bool CheckRowEmpty(int row) const;
    int FindLinesToClear();
    void ClearLines();
    bool CheckPieceValid(const PieceState piece) const;
    void MergePieceIntoBoard();
    void MakePiece(int offset_row, int offset_col);
    Shape SelectRandomPiece();
    void SetValue(const int row, const int col, const uint8_t value);
    uint8_t GetValue(const int row, const int col) const;
    void BoardClean();
    void SetNextGamePhase(const GameState game_phase);
    size_t ComputePoints() const;
    size_t GetLinesForNextLevel() const;
    void LevelUp();
};

}