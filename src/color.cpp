#include "color.h"

Color::Color(ColorSchemes color_scheme, uint8_t shade) {
    switch (color_scheme) {
        case ColorSchemes::kWhite:
            this->color_ = Schemes::kBaseColors[0];
            break;
        case ColorSchemes::kBlack:
            this->color_ = Schemes::kBlack[0];
            break;
        case ColorSchemes::kBaseColors:
            this->color_ = Schemes::kBaseColors[shade];
            break;
        case ColorSchemes::kLightColors:
            this->color_ = Schemes::kLightColors[shade];
            break;
        case ColorSchemes::kDarkColors:
            this->color_ = Schemes::kDarkColors[shade];
            break;
    }
}

Color::RGBA Color::GetColor() {
    return color_;
}
