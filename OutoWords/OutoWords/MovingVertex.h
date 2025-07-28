#pragma once
#include "Vertex.h"
#include <map>
#include <vector>
#include <array>
#include <string>
using DictMoving = std::map<std::string, std::vector<std::array<float, 3>>>;

class MovingVertex :public Vertex {

private:
	//����� �����
	DictMoving MovingData;
public:
	MovingVertex();
	MovingVertex(StateVertex StateV, const DictMoving& MovingData);
	MovingVertex(const Vertex& v, const DictMoving& MovingData);// ����� ����� ������
	const DictMoving& GetMovingData() const;
	void SetMovingData(const DictMoving& data);


};


