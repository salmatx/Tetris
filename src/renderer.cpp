#include <iostream>
#include "renderer.h"
#include "color.h"

namespace game {

Renderer::Renderer() : kScreenHeight_(720), kScreenWidth_(440) {
    this->kBackgroundColor_ = BLACK;
}

Renderer::Renderer(size_t width, size_t height)
        : kScreenHeight_(height), kScreenWidth_(width) {
    this->kBackgroundColor_ = BLACK;
}

void Renderer::InitRenderer() {
    InitWindow(this->kScreenWidth_, this->kScreenHeight_, this->kTitle_);
    this->font_ = LoadFont("../src/fonts/novem___.ttf");
    SetTargetFPS(60);
}

Renderer::~Renderer() {
    UnloadFont(this->font_);
    CloseWindow();
}

void Renderer::GameLoop(Game* game) {
    this->game_ = game;
    while (!WindowShouldClose()) {
        auto input = this->GetMoveType();
        this->game_->UpdateGame(input);
        this->RenderGame();
    }
}

void Renderer::RenderGame() const{
    BeginDrawing();
    ClearBackground(this->kBackgroundColor_);
    if (this->game_->GetActualGamePhase() == GameState::kGameStartPhase) {

    }
    if (this->game_->GetActualGamePhase() == GameState::kGamePlayPhase) {
        this->DrawBoard();
        this->DrawPiece();
    }
    if (this->game_->GetActualGamePhase() == GameState::kGameLinePhase) {
        this->DrawLineClearingHighlight();
    }
    if (this->game_->GetActualGamePhase() == GameState::kGameOverPhase) {
        size_t x = this->game_->GetBoardWidth() * this->kGridSize_ / 2;
        size_t y = this->game_->GetBoardHeight() * this->kGridSize_ / 2;
        this->DrawString(this->font_, this->font_.baseSize * 1.5, "GAME OVER", x, y, TextAlignment::kCenter, WHITE);
    }
    EndDrawing();
}

void Renderer::DrawPiece() const{
    auto piece_size = this->game_->GetPieceSize();
    auto piece_shape = this->game_->GetPiece();
    for (int i = 0; i < piece_size; ++i) {
        for (int j = 0; j < piece_size; ++j) {
            uint8_t value = *piece_shape++;
            if (value) {
                int row = this->game_->GetPieceRowPosition() + i;
                int col = this->game_->GetPieceColumnPosition() + j;
                DrawCell(row, col, value, 0, 0);
            }
        }
    }
}

void Renderer::DrawCell(int row, int col, int value, int offset_row, int offset_col) const {
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

    if (value) {
        DrawRectangle(x, y, this->kGridSize_, this->kGridSize_, dark_color);
        DrawRectangle(x + edge, y, this->kGridSize_ - edge, this->kGridSize_ - edge, light_color);
        DrawRectangle(x + edge, y + edge, this->kGridSize_ - edge * 2,
                      this->kGridSize_ - edge * 2, base_color);
    }
}

MoveTypes Renderer::GetMoveType() const {
    if (IsKeyPressed(KEY_LEFT)) return MoveTypes::kLeft;
    if (IsKeyPressed(KEY_RIGHT)) return MoveTypes::kRight;
    if (IsKeyPressed(KEY_UP)) return MoveTypes::kUp;
    if (IsKeyPressed(KEY_DOWN)) return MoveTypes::kDown;
    if (IsKeyPressed(KEY_SPACE)) return MoveTypes::kSpace;

    return MoveTypes::kNone;
}

void Renderer::DrawBoard() const{
    auto board = this->game_->GetBoard();
    for (int i = 0; i < this->game_->GetBoardHeight(); ++i) {
        for (int j = 0; j < this->game_->GetBoardWidth(); ++j) {
            uint8_t value = board.at(i).at(j);
            this->DrawCell(i, j, value, 0, 0);
        }
    }
    DrawBoardOutline();
}

void Renderer::DrawBoardOutline() const {
    int width = this->game_->GetBoardWidth() * this->kGridSize_;
    int height = this->game_->GetBoardHeight() * this->kGridSize_;
    DrawRectangleLines(0, 0, width, height, WHITE);
}

void Renderer::DrawLineClearingHighlight() const {
    for (int i = 0; i < this->game_->GetBoardHeight(); ++i) {
        if (this->game_->IsLineClearing(i)) {
            DrawRectangle(0, i * this->kGridSize_, this->kGridSize_ * this->game_->GetBoardWidth(),
                          this->kGridSize_, WHITE);
        }
    }
}

void Renderer::DrawString(Font font, float font_size, const char* msg, size_t x, size_t y, TextAlignment alignment, Color color) const {
    const int spacing = -3;
    float x_pos;
    float y_pos = (float)y;
    switch (alignment) {
        case TextAlignment::kRight:
            x_pos = (float)x - MeasureTextEx(font,msg, font_size, -3).x;
            break;
        case TextAlignment::kCenter:
            x_pos = (float)x - MeasureTextEx(font,msg, font_size, -3).x / 2;
            break;
        case TextAlignment::kLeft:
            x_pos = (float)x;
            break;
    }
    Vector2 position = {x_pos, y_pos};
    DrawTextEx(font, msg, position, font_size, -3, color);
}

}