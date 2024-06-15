#pragma once

#include <raylib.h>

namespace game {

enum class MoveType {
    kLeft, kRight, kUp, kDown, kDrop, kConfirm, kNone
};

enum class Shape {
    kSquare, kBar, kPyramid, kSShape, kZShape, kLShape, kJShape, kNumOfShapes
};

enum class GameState {
    kGameStartPhase, kGamePlayPhase, kGameLinePhase, kGameOverPhase
};

enum class TextAlignment {
    kRight, kCenter, kLeft
};

enum class PieceType {
    kActualPiece, kNextPiece
};

enum class PlayerType {
    kPlayer1, kPlayer2, kPlayerNone
};

struct PlayerMove {
    MoveType moveType;
    PlayerType player;
};

inline Color kBackgroundColor = BLACK;
inline const char* font_type = "../src/fonts/novem___.ttf";

}
