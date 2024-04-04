#include "color.h"

Color::Color(ColorSchemes color_scheme, uint8_t shade) {
    switch (color_scheme) {
        case ColorSchemes::kWhite:
            this->color_ = Schemes::white_color[0];
            break;
        case ColorSchemes::kBlack:
            this->color_ = Schemes::black_color[0];
            break;
        case ColorSchemes::kBaseColors:
            this->color_ = Schemes::base_colors[shade];
            break;
        case ColorSchemes::kLightColors:
            this->color_ = Schemes::light_colors[shade];
            break;
        case ColorSchemes::kDarkColors:
            this->color_ = Schemes::dark_colors[shade];
            break;
    }
}

Color::RGBA Color::GetColor() {
    return color_;
}
