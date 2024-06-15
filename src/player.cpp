#include <iostream>
#include "player.h"
#include "color.h"
#include "game.h"

namespace game {

Player::Player(IBoard &board, const int x_offset)
    : kMarginX_(x_offset), board_(board) {
}

void Player::DrawPlayer() const{
    if (this->board_.GetActualGamePhase() == GameState::kGamePlayPhase) {
        this->DrawPiece(PieceType::kActualPiece, this->kMarginX_, this->kMarginY_);
        this->DrawStartOverlap();
        this->DrawBoard();
        this->DrawShadowPiece();
        this->DrawGameInfo();
    }
    if (this->board_.GetActualGamePhase() == GameState::kGameLinePhase) {
        this->DrawBoard();
        this->DrawLineClearingHighlight();
        this->DrawGameInfo();
    }
    if (this->board_.GetActualGamePhase() == GameState::kGameOverPhase) {
        this->DrawBoard();
        this->DrawGameInfo();
    }
}

void Player::DrawPiece(PieceType type, const int x_offset,  const int y_offset) const{
    auto piece_size = this->board_.GetPieceSize(type);
    auto piece_shape = this->board_.GetPiece(type).get();
    for (int i = 0; i < piece_size; ++i) {
        for (int j = 0; j < piece_size; ++j) {
            uint8_t value = *piece_shape++;
            if (value) {
                int row = this->board_.GetPieceRowPosition(type) + i;
                int col = this->board_.GetPieceColumnPosition(type) + j;
                DrawCell(row, col, x_offset, y_offset, value, false);
            }
        }
    }
}

void Player::DrawCell(int row, int col, const int x_offset, const int y_offset, int value, bool outline) const {
    auto base_color_scheme = ColorScheme(ColorSchemes::kBaseColors, value);
    auto light_color_scheme = ColorScheme(ColorSchemes::kLightColors, value);
    auto dark_color_scheme = ColorScheme(ColorSchemes::kDarkColors, value);

    Color base_color {base_color_scheme.GetRValue(), base_color_scheme.GetGValue(),
                            base_color_scheme.GetBValue(), base_color_scheme.GetAValue()};
    Color light_color {light_color_scheme.GetRValue(), light_color_scheme.GetGValue(),
                       light_color_scheme.GetBValue(), light_color_scheme.GetAValue()};
    Color dark_color {dark_color_scheme.GetRValue(), dark_color_scheme.GetGValue(),
                      dark_color_scheme.GetBValue(), dark_color_scheme.GetAValue()};

    int edge = this->kGridSize_ / 8;
    int x = col * this->kGridSize_ + x_offset;
    int y = row * this->kGridSize_ + y_offset;

    if (outline) {
        DrawRectangleLines(x, y, this->kGridSize_, this->kGridSize_, base_color);
        return;
    }

    if (value) {
        DrawRectangle(x, y, this->kGridSize_, this->kGridSize_, dark_color);
        DrawRectangle(x + edge, y, this->kGridSize_ - edge, this->kGridSize_ - edge, light_color);
        DrawRectangle(x + edge, y + edge, this->kGridSize_ - edge * 2,
                      this->kGridSize_ - edge * 2, base_color);
    }
}

void Player::DrawBoard() const{
    auto board = this->board_.GetBoard();
    int board_height = this->board_.GetBoardHeight();
    int board_width = this->board_.GetBoardWidth();
    for (int i = 0; i < board_height; ++i) {
        for (int j = 0; j < board_width; ++j) {
            uint8_t value = board.at(i).at(j);
            this->DrawCell(i, j, this->kMarginX_, this->kMarginY_, value, false);
        }
    }
    DrawBoardOutline();
}

void Player::DrawBoardOutline() const {
    int width = this->board_.GetBoardWidth() * this->kGridSize_;
    int height = this->board_.GetBoardHeight() * this->kGridSize_;
    DrawRectangleLines(this->kMarginX_, this->kMarginY_, width, height, WHITE);
}

void Player::DrawLineClearingHighlight() const {
    for (int i = 0; i < this->board_.GetBoardHeight(); ++i) {
        if (this->board_.IsLineClearing(i)) {
            DrawRectangle(this->kMarginX_, i * this->kGridSize_ + this->kMarginY_, this->kGridSize_ * this->board_.GetBoardWidth(),
                          this->kGridSize_, WHITE);
        }
    }
}

void Player::DrawGameInfo() const {
    char buffer[2048];
    std::sprintf(buffer, "LEVEL: %ld", this->board_.GetLevel());
    float x = this->kMarginX_;
    float y = 0;
    float spacing = 30;
    game::DrawString(this->font_, this->font_.baseSize, buffer, x, y, TextAlignment::kLeft, WHITE);
    std::sprintf(buffer, "CLEARED LINES: %ld", this->board_.GetClearedLineCount());
    game::DrawString(this->font_, this->font_.baseSize, buffer, x + 120, y, TextAlignment::kLeft, WHITE);
    std::sprintf(buffer, "POINTS: %ld", this->board_.GetPoints());
    game::DrawString(this->font_, this->font_.baseSize, buffer, x, y + spacing, TextAlignment::kLeft, WHITE);

    x = this->kMarginX_ + 330;
    y = 60;
    game::DrawString(this->font_, this->font_.baseSize, "NEXT PIECE", x, y, TextAlignment::kLeft, WHITE);
    DrawRectangleLines(x - 20, y, 120, y + 90, WHITE);
    x = 250 - (this->board_.GetPieceSize(PieceType::kNextPiece) * this->kGridSize_) / 2 +
            this->kMarginX_;
    y = 100;
    this->DrawPiece(PieceType::kNextPiece, x, y);
}

void Player::DrawStartOverlap() const {
    DrawRectangle(this->kMarginX_, this->kMarginY_, this->board_.GetBoardWidth() * this->kGridSize_,
                  2 * this->kGridSize_, game::kBackgroundColor);
}

void Player::DrawShadowPiece() const {
    auto piece_size = this->board_.GetPieceSize(PieceType::kActualPiece);
    auto piece_shape = this->board_.GetPiece(PieceType::kActualPiece).get();
    for (int i = 0; i < piece_size; ++i) {
        for (int j = 0; j < piece_size; ++j) {
            uint8_t value = *piece_shape++;
            if (value) {
                int row = this->board_.GetShadowPieceRowPosition() + i;
                int col = this->board_.GetPieceColumnPosition(PieceType::kActualPiece) + j;
                DrawCell(row, col, this->kMarginX_, this->kMarginY_, value, true);
            }
        }
    }
}

GameState Player::UpdatePlayer(MoveType input) {
    return this->board_.UpdateGame(input);
}

void Player::SetStartLevel(size_t level) {
    this->board_.SetStartLevel(level);
}

void Player::SetFont(const Font& font) {
    this->font_ = font;
}

void Player::StartGame() {
    this->board_.StartGame();
}

void Player::PlayGame() {
    this->board_.PlayGame();
}

void Player::GameOver() {
    this->board_.GameOver();
}

json Player::SaveToJson(std::string_view path) {
    auto tmp = dynamic_cast<ISaveService*>(&this->board_);
    return tmp->SaveToJson(path);
}

}