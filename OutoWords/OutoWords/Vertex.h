#pragma once
enum StateVertex { StateNo = 0, StateYes1 = 1, StateYes2 = 2 }; class Vertex
{

private:

    Vertex* Edge[22];
    StateVertex State;
public:
    virtual ~Vertex() = default;
    Vertex();
    Vertex(StateVertex StateV);
    Vertex(const Vertex& other);//בנאי העתקה 
    StateVertex GetState();
    void SetState(StateVertex StateV);
    Vertex* GetEdge(int i) ;
    void SetEdge(int i, Vertex* Edge);


};

