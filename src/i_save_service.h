#pragma once

#include "lib/json.hpp"

namespace game {

using json = nlohmann::json;

class ISaveService {
public:
    virtual json SaveToJson(std::string_view path) = 0;
    virtual ~ISaveService() = default;
};

}
