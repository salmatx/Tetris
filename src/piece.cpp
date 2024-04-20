#include <cmath>
#include <array>
#include "piece.h"

namespace game {

const std::vector<Shape> shapes{Shape::kSquare, Shape::kBar, Shape::kPyramid,
                                Shape::kSShape, Shape::kZShape, Shape::kLShape,
                                Shape::kJShape};
std::unordered_map<Shape, std::array<std::shared_ptr<Piece>, rotations_count>> Piece::kAllRotations{};
bool Piece::all_rotations_computed_ = false;


Piece::Piece(Shape shape) {
    if (Piece::all_rotations_computed_) {
        this->tetrino_ = std::make_shared<Tetrino::TetrinoPiece>(*Piece::kAllRotations.at(shape).at(0)->tetrino_);
        this->next_ = Piece::kAllRotations.at(shape).at(1);
    }
    else {
        this->tetrino_ = std::make_shared<Tetrino::TetrinoPiece>(*Tetrino(shape).Get());
    }
}

void Piece::ComputeNextRotation(const std::shared_ptr<Piece>& rotated_piece,
                                const std::shared_ptr<Piece>& prev_piece) {
    uint16_t count = 0;
    for (int i = 0; i < rotated_piece->tetrino_->dim; i++) {
        for (int j = 0; j < rotated_piece->tetrino_->dim; j++) {
            rotated_piece->tetrino_->shape[count++] = prev_piece->tetrino_->shape[
                    (prev_piece->tetrino_->dim - j - 1) * prev_piece->tetrino_->dim + i];
        }
    }
}

void Piece::MakeAllRotations() {
    auto point_to_next = [&](int index) -> int {
        auto res = index < (rotations_count - 1) ? ++index : 0;
        return res;
    };
    for (int i = 0; i < rotations_count; ++i) {
        for (const auto& shape: shapes) {
            Piece::kAllRotations[shape][i] = std::make_shared<Piece>(Piece(shape));

            if (i) {
                Piece::kAllRotations.at(shape).at(i) = std::make_shared<Piece>(Piece(shape));
                Piece::ComputeNextRotation(Piece::kAllRotations.at(shape).at(i),
                                           Piece::kAllRotations.at(shape).at(i - 1));
            }
        }
    }
    for (int i = 0; i < rotations_count; ++i) {
        for (const auto& shape: shapes) {
            Piece::kAllRotations.at(shape).at(
                    i)->next_ = Piece::kAllRotations.at(shape).at(
                    point_to_next(i));
        }
    }
    Piece::all_rotations_computed_ = true;
}

Piece::Piece(const Piece& other) {
    this->tetrino_ = other.tetrino_;
    this->next_ = other.next_;
}

Piece& Piece::operator=(const Piece& other) {
    Piece tmp{other};
    std::swap(this->tetrino_, tmp.tetrino_);
    std::swap(this->next_, tmp.next_);
    return *this;
}

uint16_t Piece::GetDim() const {
    return this->tetrino_->dim;
}


std::weak_ptr<Piece> Piece::FastRotation() const {
    return this->next_;
}

std::shared_ptr<tetrino[]> Piece::GetPiece() const {
    return this->tetrino_->shape;
}

}