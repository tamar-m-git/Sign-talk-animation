#pragma once
#include "Vertex.h"
#include "MovingVertex.h"
#include <string>
class OutoGraph
{
private:
	Vertex* root;
	std::string* words;
public:
	OutoGraph();
	void InsertFromJsonFile(const std::string& filename);
	void Insert(const std::string& word, StateVertex Stt,const DictMoving& data);
	MovingVertex* Search(const std::string& word)const noexcept;
	void PrintWord(Vertex* root,std::string word);
	Vertex* GetRoot()const { return root; } // פונקציה שמחזירה את השורש של הגרף
	void PrintMotionDictionary(const MovingVertex* mv);//הדפסת נתוני תנועה

};

