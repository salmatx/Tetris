#include <cmath>
#include <iostream>
#include <utility>
#include "piece.h"

const std::vector<Shape> shapes{Shape::kSquare, Shape::kBar, Shape::kPyramid,
                                Shape::kSShape, Shape::kZShape,Shape::kLShape,
                                Shape::kJShape, Shape::kNumOfShapes};
std::unordered_map<Shape, std::vector<std::unique_ptr<Piece>>> Piece::kAllRotations{};


Piece::Piece(Shape shape) {
    switch (shape) {
        case Shape::kSquare:
            this->shape_ = (tetrino*) Tetrinos::square;
            this->dim_ = static_cast<uint16_t>(sqrt(
                    sizeof(Tetrinos::square) / sizeof(*Tetrinos::square)));
            break;
        case Shape::kBar:
            this->shape_ = (tetrino*) Tetrinos::bar;
            this->dim_ = static_cast<uint16_t>(sqrt(
                    sizeof(Tetrinos::bar) / sizeof(*Tetrinos::bar)));
            break;
        case Shape::kSShape:
            this->shape_ = (tetrino*) Tetrinos::s_shape;
            this->dim_ = static_cast<uint16_t>(sqrt(
                    sizeof(Tetrinos::s_shape) / sizeof(*Tetrinos::s_shape)));
            break;
        case Shape::kZShape:
            this->shape_ = (tetrino*) Tetrinos::z_shape;
            this->dim_ = static_cast<uint16_t>(sqrt(
                    sizeof(Tetrinos::z_shape) / sizeof(*Tetrinos::z_shape)));
            break;
        case Shape::kPyramid:
            this->shape_ = (tetrino*) Tetrinos::pyramid;
            this->dim_ = static_cast<uint16_t>(sqrt(
                    sizeof(Tetrinos::pyramid) / sizeof(*Tetrinos::pyramid)));
            break;
        case Shape::kLShape:
            this->shape_ = (tetrino*) Tetrinos::l_shape;
            this->dim_ = static_cast<uint16_t>(sqrt(
                    sizeof(Tetrinos::l_shape) / sizeof(*Tetrinos::l_shape)));
            break;
        case Shape::kJShape:
            this->shape_ = (tetrino*) Tetrinos::j_shape;
            this->dim_ = sizeof(Tetrinos::j_shape) / sizeof(*Tetrinos::j_shape);
            break;
        default:
            this->shape_ = nullptr;
            break;
    }
    if (Piece::kAllRotations.size() == static_cast<int>(Shape::kNumOfShapes) * rotations_count) {
        this->next_ = Piece::kAllRotations.at(shape).at(1).get();
    }
}

Piece::Piece(tetrino* shape) : shape_(shape) {}

void Piece::ComputeNextRotation(const Piece& piece, Shape shape) {
    uint16_t count = 0;
    Piece default_piece{shape};
    for (int i = 0; i < piece.dim_; i++) {
        for (int j = 0; j < piece.dim_; j++) {
            piece.shape_[count++] = default_piece.shape_[
                    (default_piece.dim_ - j - 1) * default_piece.dim_ + i];
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
            Piece::kAllRotations.at(shape).at(i)->shape_ = new tetrino[
            Piece::kAllRotations.at(shape).back()->dim_ *
            Piece::kAllRotations.at(shape).back()->dim_];
            Piece::ComputeNextRotation(*Piece::kAllRotations.at(shape).at(i),
                                       shape);
        }
    }
    for (int i = 0; i < rotations_count; ++i) {
        for (const auto& shape: shapes) {
            Piece::kAllRotations.at(shape).at(
                    i)->next_ = Piece::kAllRotations.at(shape).at(
                    point_to_next(i)).get();
        }
    }
}

Piece::Piece(const Piece& other) {
    this->shape_ = other.shape_;
    this->dim_ = other.dim_;
    this->next_ = other.next_;
}

Piece::Piece(Piece&& other) noexcept {
    this->dim_ = std::exchange(other.dim_, 0);
    this->shape_ = std::exchange(other.shape_, nullptr);
    this->next_ = std::exchange(other.next_, nullptr);
}

Piece& Piece::operator=(const Piece& other) {
    Piece tmp{other};
    std::swap(this->dim_, tmp.dim_);
    std::swap(this->shape_, tmp.shape_);
    std::swap(this->next_, tmp.next_);
    return *this;
}

Piece& Piece::operator=(Piece&& other) noexcept {
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
        for (const auto& piece: Piece::kAllRotations.at(key)) {
            delete[] piece->shape_;
        }
    }
}

Piece* Piece::FastRotation() const {
    return this->next_;
}

tetrino* Piece::GetPiece() const {
    return this->shape_;
}

uint16_t Piece::GetSize() const {
    return this->dim_ * this->dim_;
}
