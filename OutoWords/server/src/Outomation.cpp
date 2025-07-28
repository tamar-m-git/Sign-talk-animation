#include "Outomation.h"

void Outomation::LoadFromFile(const std::string& filename) {
    graph.InsertFromJsonFile(filename);
}

DictMoving Outomation::FindWord(const std::string& word) const {
    MovingVertex* mv = graph.Search(word);
    if (!mv || mv->GetState() == StateVertex::StateYes1) {
        return {};
    }
    return mv->GetMovingData();
}

void Outomation::PrintAllWords() const {
    graph.PrintWord(graph.GetRoot(), "");
}

MovingVertex* Outomation::SearchVertex(const std::string& word) const {
    return graph.Search(word);
}
