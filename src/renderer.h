#pragma once

#include <cstddef>
#include <raylib.h>
#include "game.h"

namespace game {

class Renderer {
public:
    Renderer();
    Renderer(size_t width, size_t height);
    ~Renderer();
    void InitRenderer();
    void GameLoop(Game* game);
    MoveTypes GetMoveType() const;

private:
    const size_t kScreenHeight_;
    const size_t kScreenWidth_;
    const int kGridSize_ = 30;
    const char* kTitle_ = "Tetris";
    Color kBackgroundColor_;
    Game* game_;

    void RenderGame();
    void DrawPiece();
    void DrawCell(int row, int col, int value, int offset_row, int offset_col) const;
};

}
