#include <cassert>
#include <random>
#include "board.h"

namespace game {

using RandGenType = std::mt19937;

RandGenType rand_gen;
std::uniform_int_distribution<uint32_t> uniform_dist(0, static_cast<int>(game::Shape::kNumOfShapes) - 1);

Board::Board() :
        board_(this->height_, std::vector<uint8_t>(this->width_)) {
    rand_gen.seed(time(nullptr));
    this->lines_to_clear_.reserve(this->height_);
    this->MakePiece(0, this->width_ / 2 - 1);
}

void Board::SetValue(const int& row, const int& col, const uint8_t value) {
    this->board_.at(row).at(col) = value;
}

uint8_t Board::GetValue(const int& row, const int& col) const{
    return this->board_.at(row).at(col);
}

bool Board::CheckPieceValid(const Board::PieceState& piece) {
    assert(&piece.piece);
    tetrino* shape = piece.piece.GetPiece();
    uint16_t size = piece.piece.GetDim();
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            uint8_t value = *shape++;
            if (value) {
                int board_row = piece.offset_row + i;
                int board_col = piece.offset_col + j;
                if (board_row < 0) return false;
                if (board_row >= this->height_) return false;
                if (board_col < 0) return false;
                if (board_col >= this->width_) return false;
                if (this->GetValue(board_row, board_col)) return false;
            }
        }
    }
    return true;
}

void Board::MakePiece(int offset_row, int offset_col) {
    if (this->next_piece_ == nullptr) {
        auto shape = this->SelectRandomPiece();
        this->next_piece_ = std::make_unique<PieceState>(PieceState{Piece{shape}, offset_row, offset_col});
    }
    this->actual_piece_ = std::move(this->next_piece_);
    auto next_shape = this->SelectRandomPiece();
    this->next_piece_ = std::make_unique<PieceState>(PieceState{Piece{next_shape}, offset_row, offset_col});
}

void Board::MovePieceLeft() {
    PieceState tmp = *this->actual_piece_;
    --tmp.offset_col;
    if (this->CheckPieceValid(tmp)) {
        --this->actual_piece_->offset_col;
    }
}

void Board::MovePieceRight() {
    PieceState tmp = *this->actual_piece_;
    ++tmp.offset_col;
    if (this->CheckPieceValid(tmp)) {
        ++this->actual_piece_->offset_col;
    }
}

void Board::RotatePiece() {
    PieceState tmp = *this->actual_piece_;
    tmp.piece = *tmp.piece.FastRotation();
    if (this->CheckPieceValid(tmp)) {
        this->actual_piece_->piece = *this->actual_piece_->piece.FastRotation();
    }
}

void Board::MovePiece(const MoveTypes& move) {
    switch (move) {
        case MoveTypes::kLeft:
            this->MovePieceLeft();
            break;
        case MoveTypes::kRight:
            this->MovePieceRight();
            break;
        case MoveTypes::kUp:
            this->RotatePiece();
            break;
        case MoveTypes::kDown:
            SoftDrop();
            break;
        case MoveTypes::kSpace:
            HardDrop();
        default:
            break;
    }
}

bool Board::SoftDrop() {
    ++this->actual_piece_->offset_row;
    if (!this->CheckPieceValid(*this->actual_piece_)) {
        --this->actual_piece_->offset_row;
        this->MergePieceIntoBoard();
        this->MakePiece(0, this->width_ / 2 - 1);
        return false;
    }
    return true;
}

void Board::MergePieceIntoBoard() {
    tetrino* shape = this->actual_piece_->piece.GetPiece();
    uint16_t size = this->actual_piece_->piece.GetDim();
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            uint8_t value = *shape++;
            if (value) {
                int board_row = this->actual_piece_->offset_row + i;
                int board_col = this->actual_piece_->offset_col + j;
                this->SetValue(board_row, board_col, value);
            }
        }
    }
}

Shape Board::SelectRandomPiece() {
    int number = uniform_dist( rand_gen);
    return static_cast<game::Shape>(number);
}

uint8_t* Board::GetPiece(PieceType type) {
    switch (type) {
        case PieceType::kActualPiece:
            return this->actual_piece_->piece.GetPiece();
        case PieceType::kNextPiece:
            return this->next_piece_->piece.GetPiece();
    }
    return nullptr;
}

int Board::GetPieceRowPosition(PieceType type) {
    switch (type) {
        case PieceType::kActualPiece:
            return this->actual_piece_->offset_row;
        case PieceType::kNextPiece:
            return this->next_piece_->offset_row;
    }
    return 0;
}

int Board::GetPieceColumnPosition(PieceType type) {
    switch (type) {
        case PieceType::kActualPiece:
            return this->actual_piece_->offset_col;
        case PieceType::kNextPiece:
            return this->next_piece_->offset_col;
    }
    return 0;
}

uint16_t Board::GetPieceSize(PieceType type) {
    switch (type) {
        case PieceType::kActualPiece:
            return this->actual_piece_->piece.GetDim();
        case PieceType::kNextPiece:
            return this->next_piece_->piece.GetDim();
    }
    return 0;
}

size_t Board::GetBoardHeight() {
    return this->height_;
}

size_t Board::GetBoardWidth() {
    return this->width_;
}

std::vector<std::vector<uint8_t>> Board::GetBoard() {
    return this->board_;
}

void Board::HardDrop() {
    while (SoftDrop());
}

bool Board::CheckRowFilled(const int& row) const {
    for (int i = 0; i < this->width_; ++i) {
        if (!this->GetValue(row, i)) {
            return false;
        }
    }
    return true;
}

int Board::FindLinesToClear() {
    int count = 0;
    for (int i = 0; i < this->height_; ++i) {
        this->lines_to_clear_[i] = this->CheckRowFilled(i);
        if (this->lines_to_clear_[i]) {
            ++count;
        }
    }
    return count;
}

void Board::ClearLines() {
    int src_row = this->height_ - 1;
    for (int dest_row = src_row; dest_row >= 0; --dest_row) {
        while (src_row > 0 && this->lines_to_clear_[src_row]) {
            --src_row;
        }
        if (src_row < 0) {
            std::fill(this->board_.at(dest_row).begin(),
                      this->board_.at(dest_row).end(), 0);
        }
        else {
            std::copy(this->board_.at(src_row).begin(),
                      this->board_.at(src_row).end(),
                      this->board_.at(dest_row).begin());
            --src_row;
        }
    }
}

void Board::SetPendingLineCount(uint8_t value) {
    this->pending_line_count_ = value;
}

uint8_t Board::GetPendingLineCount() {
    return this->pending_line_count_;
}

void Board::SetClearedLineCount(uint8_t value) {
    this->cleared_line_count_ = value;
}

size_t Board::GetClearedLineCount() const {
    return this->cleared_line_count_;
}

bool Board::IsLineClearing(int index) const {
    return this->lines_to_clear_[index];
}

bool Board::CheckRowEmpty(const int& row) const {
    for (int i = 0; i < this->width_; ++i) {
        if (this->GetValue(row, i)) {
            return false;
        }
    }
    return true;
}

int Board::GetShadowPieceRowPosition(){
    auto shadow_piece = *this->actual_piece_;
    while (this->CheckPieceValid(shadow_piece)) {
        ++shadow_piece.offset_row;
    }
    --shadow_piece.offset_row;
    return shadow_piece.offset_row;
}

void Board::UpdateGame(const MoveTypes& input) {
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

GameState Board::GetActualGamePhase() const {
    return this->game_phase_;
}

size_t Board::GetStartLevel() const {
    return this->start_level_;
}

size_t Board::GetLevel() const {
    return this->level_;
}

size_t Board::GetPoints() const {
    return this->points_;
}

void Board::UpdateGameplay(const MoveTypes& input) {
    this->MovePiece(input);
    if (this->time_duration_ >= this->next_drop_time_) {
        this->DropPiece();
    }
    this->SetPendingLineCount(this->FindLinesToClear());
    if (this->GetPendingLineCount() > 0) {
        this->SetNextGamePhase(GameState::kGameLinePhase);
        this->highlight_end_time_ = this->time_duration_ + 0.5f;
    }
    int game_over_row = 0;
    if (!this->CheckRowEmpty(game_over_row)) {
        this->SetNextGamePhase(GameState::kGameOverPhase);
    }
}

void Board::DropPiece() {
    this->next_drop_time_ = 0;
    if (this->SoftDrop()) {
        this->next_drop_time_ = this->time_duration_ + this->GetTimeToNextDrop();
    }
}

void Board::UpdateGameStart(const MoveTypes& input, const size_t board_width,
                            const size_t board_height) {
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
        this->BoardClean();
        this->SetNextGamePhase(GameState::kGamePlayPhase);
    }
}

void Board::UpdateGameOver(const MoveTypes& input) {
    if (input == MoveTypes::kSpace) {
        this->SetNextGamePhase(GameState::kGameStartPhase);
    }
}

void Board::UpdateGameLines() {
    if (this->time_duration_ >= this->highlight_end_time_) {
        this->ClearLines();
        this->SetClearedLineCount(this->GetClearedLineCount() +
                                          this->GetPendingLineCount());
        this->points_ += this->ComputePoints();
        this->LevelUp();
        this->SetNextGamePhase(GameState::kGamePlayPhase);
    }
}

float Board::GetTimeToNextDrop() {
    if (this->level_ > 29) {
        this->level_ = 29;
    }
    return this->kFramesPerDrop[this->level_] * this->kTargetSecondsPerFrame;
}

void Board::SetNextGamePhase(const GameState& game_phase) {
    this->game_phase_ = game_phase;
}

size_t Board::ComputePoints() {
    switch (this->GetPendingLineCount()) {
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

int Board::GetLinesForNextLevel() {
    const int max_condition = this->start_level_ * 10 - 50;
    const int min_condition = this->start_level_ * 10 - 10;
    int max = 100 > max_condition ? 100 : max_condition;
    int first_level_up_limit = min_condition < max ? min_condition : max;
    if (this->level_ == this->start_level_) {
        return first_level_up_limit;
    }
    return first_level_up_limit + (int)(this->level_ - this->start_level_) * 10;
}

void Board::LevelUp() {
    int lines_for_next_level = this->GetLinesForNextLevel();
    if ((int)this->GetClearedLineCount() >= lines_for_next_level) {
        ++this->level_;
    }
}

void Board::BoardClean() {
    Board tmp{};
    tmp.start_level_ = this->start_level_;
    *this = tmp;
}

Board::Board(const Board& other) {
    this->pending_line_count_ = other.pending_line_count_;
    this->cleared_line_count_ = other.cleared_line_count_;
    this->points_ = other.points_;
    this->level_ = other.level_;
    this->start_level_ = other.start_level_;
    this->game_phase_ = other.game_phase_;
    this->next_drop_time_ = other.next_drop_time_;
    this->time_duration_ = other.time_duration_;
    this->highlight_end_time_ = other.highlight_end_time_;
    this->start_time_ = other.start_time_;
    this->current_time_ = other.current_time_;
    this->lines_to_clear_.reserve(other.lines_to_clear_.capacity());
    std::copy(other.lines_to_clear_.begin(), other.lines_to_clear_.end(),
              std::back_inserter(this->lines_to_clear_));
    this->board_.reserve(other.board_.capacity());
    std::copy(other.board_.begin(), other.board_.end(),
              std::back_inserter(this->board_));
    this->actual_piece_ = std::make_unique<PieceState>(*other.actual_piece_);
    this->next_piece_ = std::make_unique<PieceState>(*other.next_piece_);
}

Board& Board::operator=(const Board& other) {
    Board tmp{other};
    std::swap(this->pending_line_count_, tmp.pending_line_count_);
    std::swap(this->cleared_line_count_, tmp.cleared_line_count_);
    std::swap(this->lines_to_clear_, tmp.lines_to_clear_);
    std::swap(this->board_, tmp.board_);
    std::swap(this->actual_piece_, tmp.actual_piece_);
    std::swap(this->next_piece_, tmp.next_piece_);
    std::swap(this->points_, tmp.points_);
    std::swap(this->level_, tmp.level_);
    std::swap(this->start_level_, tmp.start_level_);
    std::swap(this->game_phase_, tmp.game_phase_);
    std::swap(this->next_drop_time_, tmp.next_drop_time_);
    std::swap(this->time_duration_, tmp.time_duration_);
    std::swap(this->highlight_end_time_, tmp.highlight_end_time_);
    std::swap(this->start_time_, tmp.start_time_);
    std::swap(this->current_time_, tmp.current_time_);

    return *this;
}


}