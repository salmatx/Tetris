#pragma once

#include "common.h"

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
    Piece(Piece&& other) noexcept;
    Piece& operator=(const Piece& other);
    Piece& operator=(Piece&& other) noexcept;
    Piece* FastRotation() const;
    uint16_t GetDim() const;
    uint16_t GetSize() const;
    tetrino* GetPiece() const;
    static void MakeAllRotations();
    static void Cleanup(); //deallocates memory inside kAllRotations, to be called at the end of game

private:
    struct Tetrinos {
        static constexpr tetrino square[]{
                1, 1,
                1, 1
        };
        static constexpr tetrino bar[]{
                0, 0, 0, 0,
                2, 2, 2, 2,
                0, 0, 0, 0,
                0, 0, 0, 0
        };
        static constexpr tetrino pyramid[]{
                0, 0, 0,
                3, 3, 3,
                0, 3, 0
        };
        static constexpr tetrino s_shape[]{
                0, 4, 4,
                4, 4, 0,
                0, 0, 0
        };
        static constexpr tetrino z_shape[]{
                5, 5, 0,
                0, 5, 5,
                0, 0, 0
        };
        static constexpr tetrino l_shape[]{
                0, 6, 0,
                0, 6, 0,
                0, 6, 6
        };
        static constexpr tetrino j_shape[]{
                0, 7, 0,
                0, 7, 0,
                7, 7, 0
        };
    };
    tetrino* shape_;
    uint16_t dim_;
    Piece* next_ = nullptr;
    static bool all_rotations_computed_;
    static std::unordered_map<Shape, std::vector<std::unique_ptr<Piece>>> kAllRotations;
    static void ComputeNextRotation(Piece* rotated_piece, Piece* prev_piece);
};

}