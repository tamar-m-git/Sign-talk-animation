#pragma once
#include "Vertex.h"
#include <map>
#include <vector>
#include <array>
#include <string>
#include <nlohmann/json.hpp>

using DictMoving = std::map<std::string, std::vector<std::array<float, 3>>>;

class MovingVertex : public Vertex {
private:
    DictMoving MovingData;
public:
    MovingVertex();
    MovingVertex(StateVertex state, const DictMoving& data);
    MovingVertex(const Vertex& base, const DictMoving& data);

    const DictMoving& GetMovingData() const;
    void SetMovingData(const DictMoving& data);

    nlohmann::json ToJson(const std::string& word) const;
};
