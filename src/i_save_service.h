#pragma once

#include "json.hpp"

namespace game {

using json = nlohmann::json;

class ISaveService {
public:
    virtual json SaveToJson() = 0;
    virtual bool LoadFromJson(json obj) = 0;
    virtual ~ISaveService() = default;
};

}
