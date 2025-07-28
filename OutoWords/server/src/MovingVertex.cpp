#include "MovingVertex.h"

MovingVertex::MovingVertex()
    : Vertex(StateVertex::StateNo), MovingData() {}

MovingVertex::MovingVertex(StateVertex state, const DictMoving& data)
    : Vertex(state), MovingData(data) {}

MovingVertex::MovingVertex(const Vertex& base, const DictMoving& data)
    : Vertex(base), MovingData(data) {}

const DictMoving& MovingVertex::GetMovingData() const {
    return MovingData;
}

void MovingVertex::SetMovingData(const DictMoving& data) {
    MovingData = data;
}

nlohmann::json MovingVertex::ToJson(const std::string& word) const {
    nlohmann::json result;
    result["word"] = word;

    if (GetState() == StateVertex::StateYes2) {
        result["status"] = "found_with_motion";
        nlohmann::json motion;
        for (const auto& [bone, frames] : MovingData) {
            nlohmann::json frameList = nlohmann::json::array();
            for (const auto& point : frames) {
                frameList.push_back({ point[0], point[1], point[2] });
            }
            motion[bone] = frameList;
        }
        result["motion_data"] = motion;
    }
    else if (GetState() == StateVertex::StateYes1) {
        result["status"] = "found_no_motion";
    }
    else {
        result["status"] = "unknown";
    }

    return result;
}
