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
    void UpdateGame(const MoveTypes& input, const GameState& game_state);
    void SetBoardSize(const size_t width, const size_t height);
    uint16_t GetPieceSize();
    uint8_t* GetPiece();
    int GetPieceRowPosition();
    int GetPieceColumnPosition();

private:
    enum class GamePhase {
        kStartPhase, kPlayPhase, kLineRemovalPhase, kGameOverPhase
    };

    const uint8_t kFramesPerDrop[30]{
            48, 43, 38, 33, 28, 23, 18, 13, 8, 6,
            5, 5, 5, 4, 4, 4, 3, 3, 3, 2,
            2, 2, 2, 2, 2, 2, 2, 2, 2, 1
    };
    const float kTargetSecondsPerFrame = 1.0f / 60.0f;

    std::unique_ptr<Board> board_;
    size_t points_ = 0;
    size_t level_ = 0;
    GamePhase game_phase_ = GamePhase::kStartPhase;
    float next_drop_time_;
    float time_duration_;
    std::chrono::time_point<std::chrono::steady_clock> start_time_;
    std::chrono::time_point<std::chrono::steady_clock> current_time_;

    void UpdateGameplay(const MoveTypes& input);
    void UpdateGameStart(const MoveTypes& input);
    float GetTimeToNextDrop();
    void DropPiece();
};

}