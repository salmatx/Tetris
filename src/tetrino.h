#pragma once

#include "common.h"

#include <cstdint>
#include <memory>

namespace game {

using tetrino = uint8_t;

class Tetrino {
public:
    struct TetrinoPiece {
        std::shared_ptr<tetrino[]> shape;
        uint16_t dim;
    };

    explicit Tetrino(Shape shape);
    Tetrino(const Tetrino& other) = delete;
    Tetrino& operator=(const Tetrino& other) = delete;
    std::shared_ptr<TetrinoPiece> Get();

private:
    std::shared_ptr<TetrinoPiece> instance_;
};

}
