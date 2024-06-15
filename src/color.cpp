#include "color.h"

namespace game {

ColorScheme::ColorScheme(ColorSchemes color_scheme, uint8_t shade) {
    switch (color_scheme) {
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

uint8_t ColorScheme::GetRValue() {
    return this->color_.r;
}

uint8_t ColorScheme::GetGValue() {
    return this->color_.g;
}

uint8_t ColorScheme::GetBValue() {
    return this->color_.b;
}

uint8_t ColorScheme::GetAValue() {
    return this->color_.a;
}

}