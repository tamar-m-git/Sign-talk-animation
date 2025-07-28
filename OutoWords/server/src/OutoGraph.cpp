#include "OutoGraph.h"
#include "MovingVertex.h"
#include "Vertex.h"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <sstream>
#include <map>
#include <array>
#include <vector>
#include "Outomation.h"

OutoGraph::OutoGraph() {
    root = new Vertex();
    root->SetState(StateVertex::StateNo);
}

using json = nlohmann::json;

void OutoGraph::InsertFromJsonFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open JSON file " << filename << std::endl;
        return;
    }

    json j;
    try {
        file >> j;
    }
    catch (const json::parse_error& e) {
        std::cerr << "JSON parse error: " << e.what() << std::endl;
        return;
    }

    for (const auto& entry : j) {
        std::string word = entry.value("name", "");
        int stateInt = entry.value("State", 0);
        StateVertex st;
        switch (stateInt) {
        case 1: st = StateVertex::StateYes1; break;
        case 2: st = StateVertex::StateYes2; break;
        default: st = StateVertex::StateNo; break;
        }

        DictMoving motionData;
        if (entry.contains("motion_data") && entry["motion_data"].is_object()) {
            for (auto& [bone, arr] : entry["motion_data"].items()) {
                if (!arr.is_array()) continue;
                std::vector<std::array<float, 3>> frames;
                for (auto& frame : arr) {
                    if (frame.is_array() && frame.size() == 3) {
                        frames.push_back({
                            frame[0].get<float>(),
                            frame[1].get<float>(),
                            frame[2].get<float>()
                            });
                    }
                }
                motionData[bone] = std::move(frames);
            }
        }

        try {
            Insert(word, st, motionData);
        }
        catch (const std::exception& e) {
            std::cerr << "Error inserting '" << word << "': " << e.what() << std::endl;
        }
    }
}

void OutoGraph::Insert(const std::string& word, StateVertex Stt, const DictMoving& data) {
    if (word.empty()) {
        throw std::invalid_argument("Error: Cannot insert an empty word");
    }

    Vertex* Tmp = root;
    bool flag = false;

    for (size_t i = 0; i < word.size() - 1; i++) {
        int index = word[i] - 'a';
        if (index < 0 || index >= 22) {
            throw std::out_of_range("Error: Char out of valid range in word: " + word);
        }
        if (!flag && (Tmp->GetEdge(index) != nullptr)) {
            Tmp = Tmp->GetEdge(index);
        }
        else {
            Vertex* New = new Vertex();
            New->SetState(StateNo);
            Tmp->SetEdge(index, New);
            Tmp = New;
            flag = true;
        }
    }

    int lastIndex = word[word.size() - 1] - 'a';
    if (lastIndex < 0 || lastIndex >= 22) {
        throw std::out_of_range("Error: this Char invalid: " + word[word.size() - 1]);
    }

    Vertex* oldVertex = Tmp->GetEdge(lastIndex);
    if (oldVertex != nullptr) {
        if (oldVertex->GetState() != StateVertex::StateNo) {
            std::cerr << "Error: Word already exists: " << word << std::endl;
            return;
        }
        else {
            MovingVertex* mv = new MovingVertex(*oldVertex, data);
            mv->SetState(Stt);
            Tmp->SetEdge(lastIndex, mv);
            delete oldVertex;
        }
    }
    else {
        MovingVertex* mv = new MovingVertex(Stt, data);
        mv->SetState(Stt);
        Tmp->SetEdge(lastIndex, mv);
    }

    std::cout << "Successfully inserted word: " << word << std::endl;
}

MovingVertex* OutoGraph::Search(const std::string& word) const noexcept {
    if (word.empty()) return nullptr;

    Vertex* Tmp = root;

    for (char c : word) {
        int index = c - 'a';
        if (index < 0 || index >= 22) return nullptr;
        if (!Tmp->GetEdge(index)) return nullptr;
        Tmp = Tmp->GetEdge(index);
    }

    StateVertex state = Tmp->GetState();
    if (state == StateVertex::StateNo) return nullptr;

    DictMoving resultData;
    if (state == StateVertex::StateYes2) {
        resultData = static_cast<MovingVertex*>(Tmp)->GetMovingData();
    }

    MovingVertex* mvResult = new MovingVertex(state, resultData);
    mvResult->SetState(state);
    return mvResult;
}

void OutoGraph::PrintWord(Vertex* root, std::string word) const {
    if (!root) return;
    for (int i = 0; i < 22; i++) {
        Vertex* child = root->GetEdge(i);
        if (child) {
            if (child->GetState() == StateVertex::StateYes1 || child->GetState() == StateVertex::StateYes2) {
                std::cout << word + static_cast<char>(i + 'a') << std::endl;
            }
            PrintWord(child, word + static_cast<char>(i + 'a'));
        }
    }
}

void OutoGraph::PrintMotionDictionary(const MovingVertex* mv) {
    if (!mv) {
        std::cout << "No motion data (mv is nullptr)\n";
        return;
    }
    const DictMoving& motion = mv->GetMovingData();
    if (motion.empty()) {
        std::cout << "Motion dictionary is empty\n";
        return;
    }

    std::cout << "Motion dictionary contents:\n";
    for (const auto& [bone, frames] : motion) {
        std::cout << "Bone: " << bone << '\n';
        for (size_t i = 0; i < frames.size(); ++i) {
            const auto& p = frames[i];
            std::cout << "  Frame " << i + 1
                << ": (" << p[0] << ", " << p[1] << ", " << p[2] << ")\n";
        }
    }
}
