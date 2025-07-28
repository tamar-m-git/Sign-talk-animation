#pragma once
#include <string>
#include "OutoGraph.h"
#include "MovingVertex.h"

using DictMoving = std::map<std::string, std::vector<std::array<float, 3>>>;

class Outomation {
private:
    OutoGraph graph;
public:
    void LoadFromFile(const std::string& filename);
    DictMoving FindWord(const std::string& word) const;
    void PrintAllWords() const;
    MovingVertex* SearchVertex(const std::string& word) const;
};
