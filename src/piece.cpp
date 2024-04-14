#include <cmath>
#include <iostream>
#include <utility>
#include "piece.h"

namespace game {

const std::vector<Shape> shapes{Shape::kSquare, Shape::kBar, Shape::kPyramid,
                                Shape::kSShape, Shape::kZShape, Shape::kLShape,
                                Shape::kJShape};
std::unordered_map<Shape, std::vector<std::unique_ptr<Piece>>> Piece::kAllRotations{};
bool Piece::all_rotations_computed_ = false;


Piece::Piece(Shape shape) {
    if (Piece::all_rotations_computed_) {
        this->shape_ = Piece::kAllRotations.at(shape).at(0)->shape_;
        this->next_ = Piece::kAllRotations.at(shape).at(1).get();
        this->dim_ = Piece::kAllRotations.at(shape).at(0)->dim_;
    }
    else {
        this->shape_ = Tetrino::Get(shape)->shape;
        this->dim_ = Tetrino::Get(shape)->dim;
    }
}

Piece::Piece(tetrino* shape) : shape_(shape) {}

void Piece::ComputeNextRotation(Piece* rotated_piece, Piece* prev_piece) {
    uint16_t count = 0;
    for (int i = 0; i < rotated_piece->dim_; i++) {
        for (int j = 0; j < rotated_piece->dim_; j++) {
            rotated_piece->shape_[count++] = prev_piece->shape_[
                    (prev_piece->dim_ - j - 1) * prev_piece->dim_ + i];
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
            auto piece = std::make_unique<Piece>(shape);
            Piece::kAllRotations[shape].emplace_back(std::move(piece));

            if (i) {
                Piece::kAllRotations.at(shape).at(i)->shape_ = new tetrino[
                Piece::kAllRotations.at(shape).at(0)->dim_ *
                Piece::kAllRotations.at(shape).at(0)->dim_];
                Piece::ComputeNextRotation(Piece::kAllRotations.at(shape).at(i).get(),
                                           Piece::kAllRotations.at(shape).at(i - 1).get());
            }
        }
    }
    for (int i = 0; i < rotations_count; ++i) {
        for (const auto& shape: shapes) {
            Piece::kAllRotations.at(shape).at(
                    i)->next_ = Piece::kAllRotations.at(shape).at(
                    point_to_next(i)).get();
        }
    }
    Piece::all_rotations_computed_ = true;
}

Piece::Piece(const Piece& other) {
    this->shape_ = other.shape_;
    this->dim_ = other.dim_;
    this->next_ = other.next_;
}

Piece& Piece::operator=(const Piece& other) {
    Piece tmp{other};
    std::swap(this->dim_, tmp.dim_);
    std::swap(this->shape_, tmp.shape_);
    std::swap(this->next_, tmp.next_);
    return *this;
}

uint16_t Piece::GetDim() const {
    return this->dim_;
}

void Piece::Cleanup() {
    for (const auto& [key, value]: Piece::kAllRotations) {
        for (int i = 1; i < rotations_count; ++i) {
            delete[] Piece::kAllRotations.at(key).at(i)->shape_;
        }
    }
}

Piece* Piece::FastRotation() const {
    return this->next_;
}

tetrino* Piece::GetPiece() const {
    return this->shape_;
}

}