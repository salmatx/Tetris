#pragma once

#include <cstddef>
#include <raylib.h>
#include "board.h"

namespace game {

class Renderer {
public:
    explicit Renderer(IBoard &board);
    ~Renderer();
    void InitRenderer();
    void GameLoop();
    MoveTypes GetMoveType() const;

private:
    const size_t kScreenHeight_;
    const size_t kScreenWidth_;
    const int kGridSize_ = 30;
    const int kMarginY_ = 60;
    const char* kTitle_ = "Tetris";
    Color kBackgroundColor_{};
    IBoard &board_;
    Font font_{};

    void RenderGame() const;
    void DrawPiece(int offset_x, int offset_y, PieceType type) const;
    void DrawBoard() const;
    void DrawCell(int row, int col, int value, int offset_row, int offset_col, bool outline) const;
    void DrawBoardOutline() const;
    void DrawLineClearingHighlight() const;
    void DrawString(Font font, float font_size, const char* msg, size_t x, size_t y, TextAlignment alignment, Color color) const;
    void DrawStartScreen() const;
    void DrawGameInfo() const;
    void DrawStartOverlap() const;
    void DrawShadowPiece(int offset_x, int offset_y) const;
};

}
