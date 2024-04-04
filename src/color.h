#pragma once

#include <cstdint>

enum class ColorSchemes {kWhite, kBlack, kBaseColors, kLightColors, kDarkColors};

class Color {
public:
    struct RGBA {
        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t a;
    };

    Color(ColorSchemes color_scheme, uint8_t shade);
    RGBA GetColor();

private:
    RGBA color_;

    struct Schemes {
        static constexpr RGBA white_color[]{
                {0xFF, 0xFF, 0xFF, 0xFF}
        };
        static constexpr RGBA black_color[] = {
                {0x00, 0x00, 0x00, 0x00}
        };
        static constexpr RGBA base_colors[] = {
                {0x28, 0x28, 0x28, 0xFF},
                {0x2D, 0x99, 0x99, 0xFF},
                {0x99, 0x99, 0x2D, 0xFF},
                {0x99, 0x2D, 0x99, 0xFF},
                {0x2D, 0x99, 0x51, 0xFF},
                {0x99, 0x2D, 0x2D, 0xFF},
                {0x2D, 0x63, 0x99, 0xFF},
                {0x99, 0x63, 0x2D, 0xFF}
        };
        static constexpr RGBA light_colors[] = {
                {0x28, 0x28, 0x28, 0xFF},
                {0x44, 0xE5, 0xE5, 0xFF},
                {0xE5, 0xE5, 0x44, 0xFF},
                {0xE5, 0x44, 0xE5, 0xFF},
                {0x44, 0xE5, 0x7A, 0xFF},
                {0xE5, 0x44, 0x44, 0xFF},
                {0x44, 0x95, 0xE5, 0xFF},
                {0xE5, 0x95, 0x44, 0xFF}
        };
        static constexpr RGBA dark_colors[] = {
                {0x28, 0x28, 0x28, 0xFF},
                {0x1E, 0x66, 0x66, 0xFF},
                {0x66, 0x66, 0x1E, 0xFF},
                {0x66, 0x1E, 0x66, 0xFF},
                {0x1E, 0x66, 0x36, 0xFF},
                {0x66, 0x1E, 0x1E, 0xFF},
                {0x1E, 0x42, 0x66, 0xFF},
                {0x66, 0x42, 0x1E, 0xFF}
        };
    };
};
