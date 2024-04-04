#pragma once

#include <cstddef>

namespace game {

class Game {
public:
    enum class InputTypes {
        kLeft, kRight, kRotate, kFastDrop, kHardDrop
    };

    Game(int left_move, int right_move, int rotation, int fast_drop,
         int hard_drop);
    void GameLoop(const InputTypes& input);

private:
    enum class GamePhase {
        kStartPhase, kPlayPhase, kLineRemovalPhase, kGameOverPhase
    };

    struct Moves {
        int left;
        int right;
        int rotate;
        int fast_drop;
        int hard_drop;
    };

    Moves moves_;
    size_t points_ = 0;
    size_t level_ = 0;
    GamePhase game_phase_ = GamePhase::kStartPhase;

    void UpdateGameplay(const InputTypes& input);
};

}