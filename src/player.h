#pragma once

#include <cstddef>
#include <raylib.h>
#include "board.h"
#include "i_player.h"

namespace game {

class Player : public IPlayer{
public:
    explicit Player(IBoard &board);
    ~Player();
    void InitRenderer();
    void GameLoop();
    std::optional<MoveTypes> GetMoveType() const override;

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
