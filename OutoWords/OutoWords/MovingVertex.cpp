#include "MovingVertex.h"


MovingVertex::MovingVertex()
	: Vertex(StateVertex::StateNo)  
	, MovingData()                  
{ }

MovingVertex::MovingVertex(StateVertex StateV, const DictMoving& MovingData)
	: Vertex(StateV), MovingData(MovingData) {}

MovingVertex::MovingVertex(const Vertex & v, const DictMoving& MovingData)

	: Vertex(v), MovingData(MovingData) {}

const DictMoving& MovingVertex::GetMovingData() const
{
	return MovingData;
}

void MovingVertex::SetMovingData(const DictMoving& data)
{
	MovingData = data;
}



