#include "game.h"

namespace game {

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
        this->highlight_end_time_ = this->time_duration_ + 0.5f;
    }
    int game_over_row = 0;
    if (!this->board_->CheckRowEmpty(game_over_row)) {
        this->SetNextGamePhase(GameState::kGameOverPhase);
    }
}

float Game::GetTimeToNextDrop() {
    if (this->level_ > 29) {
        this->level_ = 29;
    }
    return this->kFramesPerDrop[this->level_] * this->kTargetSecondsPerFrame;
}

void Game::DropPiece() {
    this->next_drop_time_ = 0;
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
            this->UpdateGameStart(input);
            break;
        case GameState::kGamePlayPhase:
            this->UpdateGameplay(input);
            break;
        case GameState::kGameLinePhase:
            this->UpdateGameLines();
            break;
        case GameState::kGameOverPhase:
            this->UpdateGameOver(input);
            break;
    }
}

uint16_t Game::GetPieceSize(PieceType type) {
    return this->board_->GetPieceSize(type);
}

uint8_t* Game::GetPiece(PieceType type) {
    return this->board_->GetPiece(type);
}

int Game::GetPieceRowPosition(PieceType type) {
    return this->board_->GetRowPosition(type);
}

int Game::GetPieceColumnPosition(PieceType type) {
    return this->board_->GetColumnPosition(type);
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
    if (this->time_duration_ >= this->highlight_end_time_) {
        this->board_->ClearLines();
        this->board_->SetClearedLineCount(this->board_->GetClearedLineCount() +
                                                  this->board_->GetPendingLineCount());
        this->points_ += this->ComputePoints();
        this->LevelUp();
        this->SetNextGamePhase(GameState::kGamePlayPhase);
    }
}

void Game::SetNextGamePhase(const GameState& game_phase) {
    this->game_phase_ = game_phase;
}

size_t Game::ComputePoints() {
    switch (this->board_->GetPendingLineCount()) {
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

int Game::GetLinesForNextLevel() {
    const int max_condition = this->start_level_ * 10 - 50;
    const int min_condition = this->start_level_ * 10 - 10;
    int max = 100 > max_condition ? 100 : max_condition;
    int first_level_up_limit = min_condition < max ? min_condition : max;
    if (this->level_ == this->start_level_) {
        return first_level_up_limit;
    }
    return first_level_up_limit + (int)(this->level_ - this->start_level_) * 10;
}

void Game::LevelUp() {
    int lines_for_next_level = this->GetLinesForNextLevel();
    if ((int)this->board_->GetClearedLineCount() >= lines_for_next_level) {
        ++this->level_;
    }
}

GameState Game::GetActualGamePhase() const {
    return this->game_phase_;
}

bool Game::IsLineClearing(int index) const {
    return this->board_->IsLineClearing(index);
}

void Game::UpdateGameOver(const MoveTypes& input) {
    if (input == MoveTypes::kSpace) {
        this->SetNextGamePhase(GameState::kGameStartPhase);
    }
}

void Game::UpdateGameStart(const MoveTypes& input, const size_t board_width, const size_t board_height) {
    if (input == MoveTypes::kUp) {
        ++this->start_level_;
    }
    if (input == MoveTypes::kDown && this->start_level_ > 0) {
        --this->start_level_;
    }
    if (input == MoveTypes::kSpace) {
        this->level_ = this->start_level_;
        this->points_ = 0;
        this->start_time_ = std::chrono::steady_clock::now();
        this->SetBoardSize(board_width, board_height);
         this->SetNextGamePhase(GameState::kGamePlayPhase);
    }
}

Game::Game() {
    Piece::MakeAllRotations();
}

size_t Game::GetStartLevel() const {
    return this->start_level_;
}

size_t Game::GetLevel() const {
    return this->level_;
}

size_t Game::GetClearedLineCount() const {
    return this->board_->GetClearedLineCount();
}

size_t Game::GetPoints() const {
    return this->points_;
}

int Game::GetShadowPieceRowPosition() {
    return this->board_->GetShadowPieceRowPosition();
}

}