#pragma once

#include "common.h"
#include "board.h"

#include <cstddef>
#include <chrono>

namespace game {

class Game {
public:
    Game();
    ~Game();
    void UpdateGame(const MoveTypes& input);
    void SetBoardSize(const size_t width, const size_t height);
    uint16_t GetPieceSize(PieceType type);
    uint8_t* GetPiece(PieceType type);
    int GetPieceRowPosition(PieceType type);
    int GetPieceColumnPosition(PieceType type);
    size_t GetBoardHeight();
    size_t GetBoardWidth();
    std::vector<std::vector<uint8_t>> GetBoard();
    bool IsLineClearing(int index) const;
    GameState GetActualGamePhase() const;
    size_t GetStartLevel() const;
    size_t GetLevel() const;
    size_t GetClearedLineCount() const;
    size_t GetPoints() const;
    int GetShadowPieceRowPosition();

private:
    const uint8_t kFramesPerDrop[30]{
            48, 43, 38, 33, 28, 23, 18, 13, 8, 6,
            5, 5, 5, 4, 4, 4, 3, 3, 3, 2,
            2, 2, 2, 2, 2, 2, 2, 2, 2, 1
    };
    const float kTargetSecondsPerFrame = 1.0f / 60.0f;

    std::unique_ptr<Board> board_;
    size_t points_;
    size_t level_;
    size_t start_level_;
    GameState game_phase_ = GameState::kGameStartPhase;
    float next_drop_time_;
    float time_duration_;
    float highlight_end_time_;
    std::chrono::time_point<std::chrono::steady_clock> start_time_;
    std::chrono::time_point<std::chrono::steady_clock> current_time_;

    void UpdateGameplay(const MoveTypes& input);
    void UpdateGameStart(const MoveTypes&, const size_t board_width = 10, const size_t board_height = 22);
    void UpdateGameOver(const MoveTypes& input);
    void UpdateGameLines();
    float GetTimeToNextDrop();
    void DropPiece();
    void SetNextGamePhase(const GameState& game_phase);
    size_t ComputePoints();
    int GetLinesForNextLevel();
    void LevelUp();
};

}