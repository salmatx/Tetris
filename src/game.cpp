#include "game.h"

namespace game {

Game::Game() : board_(std::make_unique<Board>()) {
    Piece::MakeAllRotations();
    this->start_time_ = std::chrono::steady_clock::now();
}

void Game::SetBoardSize(const size_t width, const size_t height) {
    this->board_ = std::make_unique<Board>(width, height);
}

void Game::UpdateGameplay(const MoveTypes& input) {
    this->board_->MovePiece(input);
    if (this->time_duration_ >= this->next_drop_time_) {
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

void Game::UpdateGame(const MoveTypes& input, const GameState& game_state) {
    this->current_time_ = std::chrono::steady_clock::now();
    this->time_duration_ =
            std::chrono::duration_cast<std::chrono::duration<float>>
                    (this->current_time_ -this->start_time_).count();
    switch (game_state) {
        case GameState::kGameStartPhase:
            break;
        case GameState::kGamePlayPhase:
            this->UpdateGameplay(input);
            break;
        case GameState::kGameLinePhase:
            break;
        case GameState::kGameOverPhase:
            break;
    }
}

uint16_t Game::GetPieceSize() {
    return this->board_->GetPieceSize();
}

uint8_t* Game::GetPiece() {
    return this->board_->GetPiece();
}

int Game::GetPieceRowPosition() {
    return this->board_->GetRowPosition();
}

int Game::GetPieceColumnPosition() {
    return this->board_->GetColumnPosition();
}

Game::~Game() {
    Piece::Cleanup();
}

size_t Game::GetBoardHeight() {
    return this->board_->GetBoardHeight();
}

size_t Game::GetBoardWidth() {
    return this->board_->GetBoardWidth();
}

std::vector<std::vector<uint8_t>> Game::GetBoard() {
    return this->board_->GetBoard();
}

}