#pragma once

#include <cstdint>
#include <vector>
#include <memory>
#include <unordered_map>

using tetrino = uint8_t;

enum Shape {kSquare, kBar, kPyramid, kSShape, kZShape, kLShape, kJShape, kNumOfShapes};

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
        static constexpr tetrino kSquare[]{
                1, 1,
                1, 1
        };
        static constexpr tetrino kBar[]{
                0, 0, 0, 0,
                1, 1, 1, 1,
                0, 0, 0, 0,
                0, 0, 0, 0
        };
        static constexpr tetrino kPyramid[]{
                0, 0, 0,
                1, 1, 1,
                0, 1, 0
        };
        static constexpr tetrino kSShape[]{
                0, 1, 1,
                1, 1, 0,
                0, 0, 0
        };
        static constexpr tetrino kZShape[]{
                1, 1, 0,
                0, 1, 1,
                0, 0, 0
        };
        static constexpr tetrino kLShape[]{
                0, 1, 0,
                0, 1, 0,
                0, 1, 1
        };
        static constexpr tetrino kJShape[]{
                0, 1, 0,
                0, 1, 0,
                1, 1, 0
        };
    };
    tetrino* shape_;
    uint16_t dim_{};
    Piece* next_ = nullptr;
    static std::unordered_map<Shape, std::vector<std::unique_ptr<Piece>>> kAllRotations;
    static void ComputeNextRotation(const Piece& piece, Shape shape);
};
