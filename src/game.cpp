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
    this->board_->SetPendingLineCount(board_->FindLinesToClear());
    if (this->board_->GetPendingLineCount() > 0) {
        this->SetNextGamePhase(GameState::kGameLinePhase);
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
    switch (this->game_phase_) {
        case GameState::kGameStartPhase:
            break;
        case GameState::kGamePlayPhase:
            this->UpdateGameplay(input);
            break;
        case GameState::kGameLinePhase:
            this->UpdateGameLines();
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

void Game::UpdateGameLines() {
    this->highlight_end_time_ = this->time_duration_ + 0.5f;
//    if (this->time_duration_ >= this->highlight_end_time_) {
        this->board_->ClearLines();
        this->board_->SetClearedLineCount(this->board_->GetClearedLineCount() +
                                                  this->board_->GetClearedLineCount());
        this->points_ += this->ComputePoints();
        this->LevelUp();
        this->SetNextGamePhase(GameState::kGamePlayPhase);
//    }
}

void Game::SetNextGamePhase(const GameState& game_phase) {
    this->game_phase_ = game_phase;
}

size_t Game::ComputePoints() {
    switch (this->board_->GetClearedLineCount()) {
        case 1:
            return 40 * (this->level_ + 1);
        case 2:
            return 100 * (this->level_ + 1);
        case 3:
            return 300 * (this->level_ + 1);
        case 4:
            return 1200 * (this->level_ + 1);
        default:
            return 0;
    }
}

size_t Game::GetLinesForNextLevel() {
    const size_t max_condition = this->start_level_ * 10 - 50;
    const size_t min_condition = this->start_level_ * 10 - 10;
    size_t max = 100 > max_condition ? 100 : max_condition;
    size_t first_level_up_limit = min_condition > max ? min_condition : max;
    if (this->level_ == this->start_level_) {
        return first_level_up_limit;
    }
    return first_level_up_limit + (this->level_ - this->start_level_) * 10;
}

void Game::LevelUp() {
    size_t lines_for_next_level = this->GetLinesForNextLevel();
    if (this->board_->GetClearedLineCount() >= lines_for_next_level) {
        ++this->level_;
    }
}

}