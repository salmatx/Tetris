#include <cassert>
#include <random>
#include "board.h"

namespace game {

using RandGenType = std::mt19937;

RandGenType rand_gen;
std::uniform_int_distribution<uint32_t> uniform_dist(0, static_cast<int>(game::Shape::kNumOfShapes) - 1);

Board::Board(size_t width, size_t height) :
        height_(height), width_(width),
        board_(height, std::vector<uint8_t>(width)) {
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

int Board::GetRowPosition(PieceType type) {
    switch (type) {
        case PieceType::kActualPiece:
            return this->actual_piece_->offset_row;
        case PieceType::kNextPiece:
            return this->next_piece_->offset_row;
    }
    return 0;
}

int Board::GetColumnPosition(PieceType type) {
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


}