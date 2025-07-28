#include "OutoGraph.h"
#include "MovingVertex.h"
#include "Vertex.h"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <sstream>
#include <map>         
#include <array>



OutoGraph::OutoGraph()
{root = new Vertex();
root->SetState(StateVertex::StateNo);

}
using json = nlohmann::json;
// פונקציית הובפת מילה לאטוטומט עם תנועה - קריאה מקובץ json, עם זימון של פונקציית הוספה פשוטה
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

	// j אמור להיות מערך של אובייקטים
	for (const auto& entry : j) {
		// 1. קבלת השם והמצב
		std::string word = entry.value("name", "");
		int stateInt = entry.value("State", 0);
		StateVertex st;
		switch (stateInt) {
		case 1: st = StateVertex::StateYes1; break;
		case 2: st = StateVertex::StateYes2; break;
		default: st = StateVertex::StateNo; break;
		}

		// 2. פרשנות motion_data למבנה פנימי
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

		// 3. הוספה לאוטומט
		try {
			Insert(word, st, motionData);
		}
		catch (const std::exception& e) {
			std::cerr << "Error inserting '" << word << "': " << e.what() << std::endl;
		}
	}
}
// פונקציה להוספת מילה לאוטומט עם תנועה לאחר הקריאה מקובץ json

void OutoGraph::Insert(const std::string& word, StateVertex Stt, const DictMoving& data)
{
	if (word.empty()) {
		throw std::invalid_argument("Error: Cannot insert an empty word");
	}

    Vertex* Tmp = root;
    bool flag = false; 

    for (size_t i = 0; i < word.size() - 1; i++)
    {
        int index = word[i] - 'a'; 
		if (index < 0 || index >= 22) //התנאי הזה של הFOR הוא זריקת שגיאה לתוים השונים מאותיות באנגלית
		{
			throw std::out_of_range("Error: Char out of valid range in word: " + word);
		}
        if (!flag && (Tmp->GetEdge(index) != nullptr))
        {
            Tmp = Tmp->GetEdge(index);
        }
        else
        {
			Vertex* New = new Vertex();
			New->SetState(StateNo);
			Tmp->SetEdge(index, New);
			Tmp = New;
			flag = true;
        }
    }
	int lastIndex = word[word.size() - 1] - 'a';
	if (lastIndex < 0 || lastIndex >= 22) {
		throw std::out_of_range("Error:this Char invalid: " + word[word.size() - 1]);
	}
	Vertex* oldVertex = Tmp->GetEdge(lastIndex);
	// בדיקה אם יש צומת כזה באוטומט
if(oldVertex != nullptr) 
    {
		// בדיקה אם המילה קיימת באוטומט
		if (oldVertex->GetState() != StateVertex::StateNo) {
			std::cerr << "Error: Word already exists: " << word << std::endl;
			return;
		}
		else
		{//אם יש צומת כזה עם מצב לא מקבל
			MovingVertex* mv= new MovingVertex(*oldVertex, data);
			mv->SetState(Stt);
			Tmp->SetEdge(lastIndex, mv);
			std::cout << mv->GetState() << std::endl;
			delete oldVertex;
		}
	}
else
{ 	// הוספת המילה לאוטומט
	MovingVertex* mv = new MovingVertex(Stt, data);
	mv->SetState(Stt);
	Tmp->SetEdge(lastIndex, mv);
	std::cout << mv->GetState() << std::endl;

}

	std::cout << "Successfully inserted word: " << word << std::endl;
}

MovingVertex* OutoGraph::Search(const std::string& word)const noexcept
{
	if (word.empty()) return nullptr;

	Vertex* Tmp = root;

	for (char c : word) {
		int index = c - 'a';
		if (index < 0 || index >= 22 || !Tmp->GetEdge(index)) {
			return nullptr;
		}
		Tmp = Tmp->GetEdge(index);
	}

	if (Tmp->GetState() == StateVertex::StateNo)
		return nullptr;

	// החזרה ישירה של הפניה קיימת אם מדובר ב-MovingVertex
	if (Tmp->GetState() == StateVertex::StateYes2) {
		return dynamic_cast<MovingVertex*>(Tmp);
	}

	// אם מדובר במילה חוקית ללא תנועה – אפשר להחזיר אובייקט חדש או nullptr לפי הצורך שלך
	if (Tmp->GetState() == StateVertex::StateYes1) {
		// אפשר גם להחזיר nullptr אם רוצים להבדיל שאין תנועה
		return new MovingVertex(StateVertex::StateYes1, {});
	}

	return nullptr;
}
// פונקציה להדפסת המילים באוטומט
void OutoGraph::PrintWord(Vertex* root, std::string word)
{
	if (root == nullptr) {
		return;
	}
	for (int i = 0; i < 22; i++)
	{
		if (root->GetEdge(i) != nullptr)
		{
			if (root->GetEdge(i)->GetState() == StateVertex::StateYes1 || root->GetEdge(i)->GetState() == StateVertex::StateYes2)
			{
				std::cout << word + (char)(i + 'a') << std::endl;
			}
			PrintWord(root->GetEdge(i), word + (char)(i + 'a'));
		}
	}
	
}
//void OutoGraph::PrintMotionData(const OutoGraph& graph, const std::string& word)
//{ 
//	MovingVertex* mv = OutoGraph::Search(word);
//	if (mv != nullptr) {
//		std::cout << "Motion data for word '" << word << "':" << std::endl;
//		for (const auto& [bone, frames] : mv->GetMovingData()) {
//			std::cout << "Bone: " << bone << std::endl;
//			for (const auto& frame : frames) {
//				std::cout << "Frame: (" << frame[0] << ", " << frame[1] << ", " << frame[2] << ")" << std::endl;
//			}
//		}
//	}
//	else {
//		std::cerr << "Error: Word not found or no motion data available." << std::endl;
//	}
//}
//
//פונקציה להדפסת נתוני תנועה למילה
void OutoGraph::PrintMotionDictionary(const MovingVertex* mv)
{
	if (!mv)
	{
		std::cout << "No motion data (mv is nullptr)\n";
		return;
	}
	 const DictMoving& motion = mv->GetMovingData();
	if (motion.empty())
	{
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

