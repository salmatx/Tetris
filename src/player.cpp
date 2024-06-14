#include <iostream>
#include "player.h"
#include "color.h"

namespace game {

Player::Player(IBoard &board) : kScreenHeight_(720), kScreenWidth_(440), board_(board) {
    this->kBackgroundColor_ = BLACK;
}

void Player::InitRenderer() {
    InitWindow(this->kScreenWidth_, this->kScreenHeight_, this->kTitle_);
    this->font_ = LoadFont("../src/fonts/novem___.ttf");
    SetTargetFPS(60);
}

Player::~Player() {
    UnloadFont(this->font_);
    CloseWindow();
}

void Player::GameLoop() {
    while (!WindowShouldClose()) {
        auto input = this->GetMoveType().value();
        this->board_.UpdateGame(input);
        this->RenderGame();
    }
}

void Player::RenderGame() const{
    BeginDrawing();
    ClearBackground(this->kBackgroundColor_);
    if (this->board_.GetActualGamePhase() == GameState::kGameStartPhase) {
        this->DrawStartScreen();
    }
    if (this->board_.GetActualGamePhase() == GameState::kGamePlayPhase) {
        this->DrawPiece(0, this->kMarginY_, PieceType::kActualPiece);
        this->DrawStartOverlap();
        this->DrawBoard();
        this->DrawShadowPiece(0, this->kMarginY_);
        this->DrawGameInfo();
    }
    if (this->board_.GetActualGamePhase() == GameState::kGameLinePhase) {
        this->DrawBoard();
        this->DrawLineClearingHighlight();
        this->DrawGameInfo();
    }
    if (this->board_.GetActualGamePhase() == GameState::kGameOverPhase) {
        this->DrawBoard();
        size_t x = this->board_.GetBoardWidth() * this->kGridSize_ / 2;
        size_t y = (this->board_.GetBoardHeight() * this->kGridSize_ + this->kMarginY_) / 2;
        this->DrawString(this->font_, this->font_.baseSize * 1.5, "GAME OVER", x, y, TextAlignment::kCenter, WHITE);
    }
    EndDrawing();
}

void Player::DrawPiece(int offset_x, int offset_y, PieceType type) const{
    auto piece_size = this->board_.GetPieceSize(type);
    auto piece_shape = this->board_.GetPiece(type).get();
    for (int i = 0; i < piece_size; ++i) {
        for (int j = 0; j < piece_size; ++j) {
            uint8_t value = *piece_shape++;
            if (value) {
                int row = this->board_.GetPieceRowPosition(type) + i;
                int col = this->board_.GetPieceColumnPosition(type) + j;
                DrawCell(row, col, value, offset_y, offset_x, false);
            }
        }
    }
}

void Player::DrawCell(int row, int col, int value, int offset_row, int offset_col, bool outline) const {
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
    int x = col * this->kGridSize_ + offset_col;
    int y = row * this->kGridSize_ + offset_row;

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

std::optional<MoveTypes> Player::GetMoveType() const {
    double wait_for_key_down = 0.05;
    if (IsKeyPressed(KEY_LEFT) && IsKeyReleased(KEY_LEFT))
        return MoveTypes::kLeft;
    if (IsKeyPressed(KEY_RIGHT) && IsKeyReleased(KEY_RIGHT))
        return MoveTypes::kRight;
    if (IsKeyPressed(KEY_UP))
        return MoveTypes::kUp;
    if (IsKeyPressed(KEY_DOWN))
        return MoveTypes::kDown;
    if (IsKeyPressed(KEY_SPACE))
        return MoveTypes::kSpace;
    if (IsKeyDown(KEY_LEFT)) {
        WaitTime(wait_for_key_down);
        return MoveTypes::kLeft;
    }
    if (IsKeyDown(KEY_RIGHT)) {
        WaitTime(wait_for_key_down);
        return MoveTypes::kRight;
    }
    return std::nullopt;
}

void Player::DrawBoard() const{
    auto board = this->board_.GetBoard();
    int board_height = this->board_.GetBoardHeight();
    int board_width = this->board_.GetBoardWidth();
    for (int i = 0; i < board_height; ++i) {
        for (int j = 0; j < board_width; ++j) {
            uint8_t value = board.at(i).at(j);
            this->DrawCell(i, j, value, this->kMarginY_, 0, false);
        }
    }
    DrawBoardOutline();
}

void Player::DrawBoardOutline() const {
    int width = this->board_.GetBoardWidth() * this->kGridSize_;
    int height = this->board_.GetBoardHeight() * this->kGridSize_;
    DrawRectangleLines(0, this->kMarginY_, width, height, WHITE);
}

void Player::DrawLineClearingHighlight() const {
    for (int i = 0; i < this->board_.GetBoardHeight(); ++i) {
        if (this->board_.IsLineClearing(i)) {
            DrawRectangle(0, i * this->kGridSize_ + this->kMarginY_, this->kGridSize_ * this->board_.GetBoardWidth(),
                          this->kGridSize_, WHITE);
        }
    }
}

void Player::DrawString(Font font, float font_size, const char* msg, size_t x, size_t y, TextAlignment alignment, Color color) const {
    const int spacing = -2;
    float x_pos;
    float y_pos = (float)y;
    switch (alignment) {
        case TextAlignment::kRight:
            x_pos = (float)x - MeasureTextEx(font,msg, font_size, spacing).x;
            break;
        case TextAlignment::kCenter:
            x_pos = (float)x - MeasureTextEx(font,msg, font_size, spacing).x / 2;
            break;
        case TextAlignment::kLeft:
            x_pos = (float)x;
            break;
    }
    Vector2 position = {x_pos, y_pos};
    DrawTextEx(font, msg, position, font_size, spacing, color);
}

void Player::DrawStartScreen() const {
    char buffer[2048];
    std::sprintf(buffer, "START LEVEL: %ld", this->board_.GetStartLevel());
    float x = this->kScreenWidth_ / 2.0f;
    float y = this->kScreenHeight_ / 2.0f - 100.0f;
    float spacing = 30;
    this->DrawString(this->font_, this->font_.baseSize * 1.5, "TETRIS", x, y, TextAlignment::kCenter, WHITE);
    y += spacing;
    this->DrawString(this->font_, this->font_.baseSize * 1.5, buffer, x, y, TextAlignment::kCenter, WHITE);
    y += 3 * spacing;
    this->DrawString(this->font_, this->font_.baseSize * 1.2, "CONTROLS", x, y, TextAlignment::kCenter, WHITE);
    x = this->kScreenWidth_ / 2.0f - 120;
    y += 2 * spacing;
    this->DrawString(this->font_, this->font_.baseSize, "SELECT LEVEL:", x, y, TextAlignment::kLeft, WHITE);
    this->DrawString(this->font_, this->font_.baseSize, "UP KEY / DOWN KEY", x + 120, y, TextAlignment::kLeft, WHITE);
    y += spacing;
    this->DrawString(this->font_, this->font_.baseSize, "MOVE:", x, y, TextAlignment::kLeft, WHITE);
    this->DrawString(this->font_, this->font_.baseSize, "LEFT KEY / RIGHT KEY", x + 120, y, TextAlignment::kLeft, WHITE);
    y += spacing;
    this->DrawString(this->font_, this->font_.baseSize, "ROTATE:", x, y, TextAlignment::kLeft, WHITE);
    this->DrawString(this->font_, this->font_.baseSize, "UP KEY", x + 120, y, TextAlignment::kLeft, WHITE);
    y += spacing;
    this->DrawString(this->font_, this->font_.baseSize, "FAST DROP:", x, y, TextAlignment::kLeft, WHITE);
    this->DrawString(this->font_, this->font_.baseSize, "DOWN KEY", x + 120, y, TextAlignment::kLeft, WHITE);
    y += spacing;
    this->DrawString(this->font_, this->font_.baseSize, "HARD DROP:", x, y, TextAlignment::kLeft, WHITE);
    this->DrawString(this->font_, this->font_.baseSize, "SPACE KEY", x + 120, y, TextAlignment::kLeft, WHITE);
}

void Player::DrawGameInfo() const {
    char buffer[2048];
    std::sprintf(buffer, "LEVEL: %ld", this->board_.GetLevel());
    float x = 0;
    float y = 0;
    float spacing = 30;
    this->DrawString(this->font_, this->font_.baseSize, buffer, x, y, TextAlignment::kLeft, WHITE);
    std::sprintf(buffer, "CLEARED LINES: %ld", this->board_.GetClearedLineCount());
    this->DrawString(this->font_, this->font_.baseSize, buffer, x + 120, y, TextAlignment::kLeft, WHITE);
    std::sprintf(buffer, "POINTS: %ld", this->board_.GetPoints());
    this->DrawString(this->font_, this->font_.baseSize, buffer, x, y + spacing, TextAlignment::kLeft, WHITE);

    x = 330;
    y = 60;
    this->DrawString(this->font_, this->font_.baseSize, "NEXT PIECE", x, y, TextAlignment::kLeft, WHITE);
    DrawRectangleLines(x - 20, y, 120, y + 90, WHITE);
    x = 250 - (this->board_.GetPieceSize(PieceType::kNextPiece) * this->kGridSize_) / 2;
    y = 100;
    this->DrawPiece(x, y, PieceType::kNextPiece);
}

void Player::DrawStartOverlap() const {
    DrawRectangle(0, this->kMarginY_, this->board_.GetBoardWidth() * this->kGridSize_,
                  2 * this->kGridSize_, this->kBackgroundColor_);
}

void Player::DrawShadowPiece(int offset_x, int offset_y) const {
    auto piece_size = this->board_.GetPieceSize(PieceType::kActualPiece);
    auto piece_shape = this->board_.GetPiece(PieceType::kActualPiece).get();
    for (int i = 0; i < piece_size; ++i) {
        for (int j = 0; j < piece_size; ++j) {
            uint8_t value = *piece_shape++;
            if (value) {
                int row = this->board_.GetShadowPieceRowPosition() + i;
                int col = this->board_.GetPieceColumnPosition(PieceType::kActualPiece) + j;
                DrawCell(row, col, value, offset_y, offset_x, true);
            }
        }
    }
}

}