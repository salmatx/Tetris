#include <chrono>
#include "game.h"

namespace game {

Game::Game(int left_move, int right_move, int rotation, int fast_drop,
           int hard_drop) : board_(std::make_unique<Board>()) {
    this->moves_.left = left_move;
    this->moves_.right = right_move;
    this->moves_.rotate = rotation;
    this->moves_.fast_drop = fast_drop;
    this->moves_.hard_drop = hard_drop;

    this->start_time_ = std::chrono::steady_clock::now();
}

void Game::SetBoardSize(const size_t width, const size_t height) {
    this->board_ = std::make_unique<Board>(width, height);
}

void Game::UpdateGameplay(const MoveTypes& input) {
    this->board_->MovePiece(input);
    while (this->time_duration_ >= this->next_drop_time_) {
        this->DropPiece();
    }
}

float Game::GetTimeToNextDrop() {
    if (this->level_ > 29) {
        this->level_ = 29;
    }
    return this->kFramesPerDrop[this->level_] * this->kTargetSecondsPerFrame;
}

void Game::DropPiece() {
    if (this->board_->SoftDrop()) {
        this->next_drop_time_ = this->time_duration_ + this->GetTimeToNextDrop();
    }

}

void Game::UpdateGame(const MoveTypes& input) {
    this->current_time_ = std::chrono::steady_clock::now();
    this->time_duration_ =
            std::chrono::duration_cast<std::chrono::duration<float>>
                    (this->current_time_ -this->start_time_).count();
}
}