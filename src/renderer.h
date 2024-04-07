#pragma once

#include <cstddef>
#include "raylib.h"

namespace game {

class Renderer {
public:
    Renderer() = default;
    Renderer(size_t width, size_t height);

private:
    const size_t kScreenWidth_ = 720;
    const size_t kScreenHeight_ = 440;
};

}
