#pragma once

namespace game {

enum class MoveTypes {
    kLeft, kRight, kUp, kDown, kSpace, kNone
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

}
