#pragma once

#include "lib/json.hpp"

namespace game {

using json = nlohmann::json;

class ISaveService {
public:
    virtual json SaveToJson() = 0;
    virtual ~ISaveService() = default;
};

}
