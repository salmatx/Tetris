#include <cassert>
#include <random>
#include "board.h"

namespace game {

using RandGenType = std::mt19937;

RandGenType rand_gen;
std::uniform_int_distribution<uint32_t> uniform_dist(0, static_cast<int>(game::Shape::kNumOfShapes));

Board::Board(size_t width, size_t height) :
        height_(height), width_(width),
        board_(width, std::vector<uint8_t>(height)) {
    rand_gen.seed(time(nullptr));
}

Board::Board() : board_(width_, std::vector<uint8_t>(height_)) {
    rand_gen.seed(time(nullptr));
}

void Board::SetValue(const int& row, const int& col, const uint8_t value) {
    this->board_.at(col).at(row) = value;
}

uint8_t Board::GetValue(const int& row, const int& col) {
    return this->board_.at(col).at(row);
}

bool Board::CheckPieceValid(const Board::PieceState& piece) {
    assert(&piece.piece);
    tetrino* shape = piece.piece.GetPiece();
    uint16_t size = piece.piece.GetSize();
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            uint8_t value = *shape++;
            if (value) {
                int board_row = piece.offset_row + i;
                int board_col = piece.offset_row + j;
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

void Board::MakePiece(Shape shape, int offset_row, int offset_col) {
    this->actual_piece_ = std::make_unique<PieceState>(PieceState{Piece{shape}, offset_row, offset_col});
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
    tmp.piece.FastRotation();
    if (this->CheckPieceValid(tmp)) {
        this->actual_piece_->piece.FastRotation();
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
        case MoveTypes::kRotate:
            this->RotatePiece();
            break;
        default:
            break;
    }
}

void Board::SoftDrop() {
    ++this->actual_piece_->offset_row;
    if (!this->CheckPieceValid(*this->actual_piece_)) {
        --this->actual_piece_->offset_row;
        this->MergePieceIntoBoard();
        this->MakePiece(this->SelectRandomPiece(), 0, this->width_ / 2);
    }
}

void Board::MergePieceIntoBoard() {
    tetrino* shape = this->actual_piece_->piece.GetPiece();
    uint16_t size = this->actual_piece_->piece.GetSize();
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            uint8_t value = *shape++;
            if (value) {
                int board_row = this->actual_piece_->offset_row + i;
                int board_col = this->actual_piece_->offset_row + j;
                this->SetValue(board_row, board_col, value);
            }
        }
    }
}

Shape Board::SelectRandomPiece() {
    int number = uniform_dist( rand_gen);
    return static_cast<game::Shape>(number);
}

}