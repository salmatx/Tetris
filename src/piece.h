#pragma once

#include "common.h"
#include "tetrino.h"

#include <cstdint>
#include <vector>
#include <memory>
#include <unordered_map>

namespace game {

using tetrino = uint8_t;

constexpr uint8_t rotations_count = 4; //rotating by 90deg

class Piece {
public:
    explicit Piece(Shape shape);
    Piece(const Piece& other);
    Piece& operator=(const Piece& other);
    std::weak_ptr<Piece> FastRotation() const;
    uint16_t GetDim() const;
    std::shared_ptr<tetrino[]> GetPiece() const;
    static void MakeAllRotations();

private:
    std::shared_ptr<Tetrino::TetrinoPiece> tetrino_;
    std::weak_ptr<Piece> next_;
    static bool all_rotations_computed_;
    static std::unordered_map<Shape, std::array<std::shared_ptr<Piece>, rotations_count>> kAllRotations;
    static void ComputeNextRotation(const std::shared_ptr<Piece>& rotated_piece,
                                    const std::shared_ptr<Piece>& prev_piece);
};

}