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
    explicit Piece(tetrino* shape);
    Piece(const Piece& other);
    Piece& operator=(const Piece& other);
    Piece* FastRotation() const;
    uint16_t GetDim() const;
    tetrino* GetPiece() const;
    static void MakeAllRotations();
    static void Cleanup(); //deallocates memory inside kAllRotations, to be called at the end of game

private:
    tetrino* shape_;
    uint16_t dim_;
    Piece* next_ = nullptr;
    static bool all_rotations_computed_;
    static std::unordered_map<Shape, std::vector<std::unique_ptr<Piece>>> kAllRotations;
    static void ComputeNextRotation(Piece* rotated_piece, Piece* prev_piece);
};

}