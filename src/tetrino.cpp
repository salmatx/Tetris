#include "tetrino.h"

#include <algorithm>

namespace game {

Tetrino::Tetrino(Shape shape) {
    switch (shape) {
        case Shape::kSquare: {
            this->instance_ = std::make_shared<TetrinoPiece>(
                    std::make_shared<tetrino[]>(4), 2);
            tetrino tmp[]{1, 1,
                          1, 1};
            std::ranges::copy(tmp, this->instance_->shape.get());
            break;
        }
        case Shape::kBar: {
            this->instance_ = std::make_shared<TetrinoPiece>(
                    std::make_shared<tetrino[]>(16), 4);
            tetrino tmp[]{0, 0, 0, 0,
                          2, 2, 2, 2,
                          0, 0, 0, 0,
                          0, 0, 0, 0};
            std::ranges::copy(tmp, this->instance_->shape.get());
            break;
        };
        case Shape::kPyramid: {
            this->instance_ = std::make_shared<TetrinoPiece>(
                    std::make_shared<tetrino[]>(9), 3);
            tetrino tmp[]{0, 0, 0,
                          3, 3, 3,
                          0, 3, 0};
            std::ranges::copy(tmp, this->instance_->shape.get());
            break;
        }
        case Shape::kSShape: {
            this->instance_ = std::make_shared<TetrinoPiece>(
                    std::make_shared<tetrino[]>(9), 3);
            tetrino tmp[]{0, 4, 4,
                          4, 4, 0,
                          0, 0, 0};
            std::ranges::copy(tmp, this->instance_->shape.get());
            break;
        }
        case Shape::kZShape: {
            this->instance_ = std::make_shared<TetrinoPiece>(
                    std::make_shared<tetrino[]>(9), 3);
            tetrino tmp[]{5, 5, 0,
                          0, 5, 5,
                          0, 0, 0};
            std::ranges::copy(tmp, this->instance_->shape.get());
            break;
        }
        case Shape::kLShape: {
            this->instance_ = std::make_shared<TetrinoPiece>(
                    std::make_shared<tetrino[]>(9), 3);
            tetrino tmp[]{0, 6, 0,
                          0, 6, 0,
                          0, 6, 6};
            std::ranges::copy(tmp, this->instance_->shape.get());
            break;
        }
        case Shape::kJShape: {
            this->instance_ = std::make_shared<TetrinoPiece>(
                    std::make_shared<tetrino[]>(9), 3);
            tetrino tmp[]{0, 7, 0,
                          0, 7, 0,
                          7, 7, 0};
            std::ranges::copy(tmp, this->instance_->shape.get());
            break;
        }
        default:
            this->instance_ = std::make_shared<TetrinoPiece>();
            break;
    }
}

std::shared_ptr<Tetrino::TetrinoPiece> Tetrino::Get() {
    return std::move(this->instance_);
}

}