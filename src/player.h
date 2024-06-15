#pragma once

#include <cstddef>
#include <raylib.h>
#include "board.h"
#include "i_player.h"

namespace game {

class Player : public IPlayer{
public:
    explicit Player(IBoard &board, const int x_offset);
    void DrawPlayer() const override;
    GameState UpdatePlayer(MoveType input, GameState game_state) override;
    void SetStartLevel(size_t level) override;
    void SetFont(const Font &font) override;

private:
    const int kGridSize_ = 30;
    const int kMarginY_ = 60;
    const int kMarginX_;
    IBoard &board_;
    Font font_{};

    void DrawPiece(PieceType type, const int x_offset, const int y_offset) const;
    void DrawBoard() const;
    void DrawCell(int row, int col, const int x_offset, const int y_offset, int value, bool outline) const;
    void DrawBoardOutline() const;
    void DrawLineClearingHighlight() const;
    void DrawGameInfo() const;
    void DrawStartOverlap() const;
    void DrawShadowPiece() const;
};

}
