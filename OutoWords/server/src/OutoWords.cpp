// OutoWords.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Vertex.h"
#include "MovingVertex.h"
#include "OutoGraph.h"
int main()
{
    int x = 2;
    std::cout << x ;
    std::cout << "Hello World!\n";
    OutoGraph* G = new OutoGraph();
    G->InsertFromJsonFile("C:\\CodesProject\\TmpFiles\\Moving.json");
    std::string word = "";
    G->PrintWord(G->GetRoot(), word);
    std::string word1 = "bbs";
    MovingVertex * mv = new MovingVertex();
    mv = G->Search(word1);
    if (mv == nullptr)//אין מילה כזו באוטומט
    {
        std::cerr << "Error: Vertex not found." << std::endl;

    }
 
    else
    { if (mv->GetState() == StateVertex::StateYes1)// מילה קיימת אך ללא תנועה
		{
			std::cout << "Found word: " << word1 << std::endl;
		}
		else//יש תנועה למילה
		{
			std::cout << "Found word: " << word1 << std::endl;
            G->PrintMotionDictionary(mv);//הדפסת תנועות המילה
		}
        
    }



    
    
   
  //  std::string word1 = "ulfm";
  //  std::string word2 = "ulf";
  //  std::string word3 = "bbsu";
  //  MovingVertex* mv = new MovingVertex();
  //  MovingVertex* mv2 = new MovingVertex();
  //  MovingVertex* mv3 = new MovingVertex();
  //  mv = G->Search(word1);
  //  mv2 = G->Search(word2);
  //  mv3 = G->Search(word3);
  // std::cout <<mv->GetState() << std::endl;
  // std::cout << mv->GetState() << std::endl;
  // std::cout << mv->GetState() << std::endl;
  //  // ... (לאחר קריאה ל-G->Search(word1) והשמה ל-mv) ...
  //if(mv2 != nullptr)
  //    std::cout << "Found word: " << word2 << std::endl;
  //else
  //    std::cout << "Word not found: " << word2 << std::endl;
  //if(mv3 != nullptr)
  //    std::cout << "Found word: " << word3 << std::endl;
  //else
  //    std::cout << "Word not found: " << word3 << std::endl;
  //  std::cin.get(); 
  //  return 0;
    
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
