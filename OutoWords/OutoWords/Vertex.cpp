#include "Vertex.h"
#include <iostream>


Vertex::Vertex():
	State(StateVertex::StateNo)
{
	for (int i = 0; i < 22; i++)
	{
		Edge[i] = nullptr;
	}
}
Vertex::Vertex(StateVertex StateV) :
	State(StateV)
{ for (int i = 0; i < 22; i++)
	{
		Edge[i] = nullptr;
	}
}
Vertex::Vertex(const Vertex& other)
{
	State = other.State;
	for (int i = 0; i < 22; i++) {
		Edge[i] = other.Edge[i];
	}

}
Vertex* Vertex::GetEdge(int i)
{
	return this->Edge[i];
}
void Vertex::SetEdge(int i, Vertex* Edge)
{
	this->Edge[i] = Edge;
}
StateVertex Vertex::GetState()
{
	return this->State;
}
void Vertex::SetState(StateVertex StateV)
{
	this->State = StateV;
}
