#pragma once

#include "common.h"

#include <cstdint>

namespace game {

using tetrino = uint8_t;

class Tetrino {
public:
    struct TetrinoPiece {
        tetrino* shape;
        uint16_t dim;
    };

    Tetrino();
    Tetrino(const Tetrino& other) = delete;
    Tetrino& operator=(const Tetrino& other) = delete;
    static const TetrinoPiece* Get(Shape shape);

private:
    static Tetrino* instance_;

    const TetrinoPiece square_;
    const TetrinoPiece bar_;
    const TetrinoPiece pyramid_;
    const TetrinoPiece s_shape_;
    const TetrinoPiece z_shape_;
    const TetrinoPiece l_shape_;
    const TetrinoPiece j_shape_;
};

}
