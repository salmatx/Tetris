#include <cassert>
#include "board.h"

namespace game {

Board::Board(size_t width, size_t height) :
        height_(height), width_(width),
        board_(width, std::vector<uint8_t>(height)) {}

Board::Board() : board_(width_, std::vector<uint8_t>(height_)) {}

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
            if (value > 0) {
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

}