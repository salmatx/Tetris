#include "tetrino.h"

namespace game {

Tetrino* Tetrino::instance_;

Tetrino::Tetrino() :
        square_(TetrinoPiece{new tetrino[]{1, 1,
                                           1, 1}, 2}),

        bar_(TetrinoPiece{new tetrino[]{0, 0, 0, 0,
                                        2, 2, 2, 2,
                                        0, 0, 0, 0,
                                        0, 0, 0, 0}, 4}),

        pyramid_(TetrinoPiece{new tetrino[]{0, 0, 0,
                                            3, 3, 3,
                                            0, 3, 0}, 3}),

        s_shape_(TetrinoPiece{new tetrino[]{0, 4, 4,
                                            4, 4, 0,
                                            0, 0, 0}, 3}),

        z_shape_(TetrinoPiece{new tetrino[]{5, 5, 0,
                                            0, 5, 5,
                                            0, 0, 0}, 3}),

        l_shape_(TetrinoPiece{new tetrino[]{0, 6, 0,
                                            0, 6, 0,
                                            0, 6, 6}, 3}),

        j_shape_(TetrinoPiece{new tetrino[]{0, 7, 0,
                                            0, 7, 0,
                                            7, 7, 0}, 3}) {}

const Tetrino::TetrinoPiece* Tetrino::Get(Shape shape) {
    if (Tetrino::instance_ == nullptr) {
        Tetrino::instance_ = new Tetrino();
    }
    switch (shape) {
        case Shape::kSquare:
            return &Tetrino::instance_->square_;
        case Shape::kBar:
            return &Tetrino::instance_->bar_;
        case Shape::kPyramid:
            return &Tetrino::instance_->pyramid_;
        case Shape::kSShape:
            return &Tetrino::instance_->s_shape_;
        case Shape::kZShape:
            return &Tetrino::instance_->z_shape_;
        case Shape::kLShape:
            return &Tetrino::instance_->l_shape_;
        case Shape::kJShape:
            return &Tetrino::instance_->j_shape_;
        default:
            return nullptr;
    }
}
}