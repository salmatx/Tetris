#include "renderer.h"
#include "color.h"

namespace game {

Renderer::Renderer() : kScreenHeight_(720), kScreenWidth_(440) {
    auto bgr_color = ColorScheme(ColorSchemes::kBlack);
    this->kBackgroundColor_ = {bgr_color.GetRValue(), bgr_color.GetGValue(),
                               bgr_color.GetBValue(), bgr_color.GetAValue()};
}

Renderer::Renderer(size_t width, size_t height)
        : kScreenWidth_(width), kScreenHeight_(height) {
    auto bgr_color = ColorScheme(ColorSchemes::kBlack);
    this->kBackgroundColor_ = {bgr_color.GetRValue(), bgr_color.GetGValue(),
                               bgr_color.GetBValue(), bgr_color.GetAValue()};
}

void Renderer::InitRenderer() {
    InitWindow(this->kScreenWidth_, this->kScreenHeight_, this->kTitle_);
    SetTargetFPS(60);
}

Renderer::~Renderer() {
    CloseWindow();
}

void Renderer::GameLoop(Game* game) {
    this->game_ = game;
    while (!WindowShouldClose()) {
        auto input = this->GetMoveType();
        this->game_->UpdateGame(input, GameState::kGamePlayPhase);
        RenderGame();
    }
}

void Renderer::RenderGame() {
    BeginDrawing();
    ClearBackground(this->kBackgroundColor_);
    this->DrawPiece();
    EndDrawing();
}

void Renderer::DrawPiece() {
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

    DrawRectangle(x, y, this->kGridSize_, this->kGridSize_, dark_color);
    DrawRectangle(x + edge, y, this->kGridSize_ - edge, this->kGridSize_ - edge, light_color);
    DrawRectangle(x + edge, y + edge, this->kGridSize_ - edge * 2,
                  this->kGridSize_ - edge * 2, base_color);
}

MoveTypes Renderer::GetMoveType() const {
    if (IsKeyPressed(KEY_LEFT)) return MoveTypes::kLeft;
    if (IsKeyPressed(KEY_RIGHT)) return MoveTypes::kRight;
    if (IsKeyPressed(KEY_UP)) return MoveTypes::kUp;
    if (IsKeyPressed(KEY_DOWN)) return MoveTypes::kDown;
    if (IsKeyPressed(KEY_SPACE)) return MoveTypes::kSpace;

    return MoveTypes::kNone;
}

}