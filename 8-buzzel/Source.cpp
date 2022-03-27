#include <iostream>
#include "EightPuzzle.h"
#include <stdlib.h> 

int main()
{
	// example state : 508421736 -> solvable

	std::string start;
	while (start.size() != 9)
	{
		std::cout << "enter the start state random combination of [0 - 8]: ";
		std::cin >> start;
	}
	
	int startState[9];

	for (int i = 0; i < 9; i++)
	{
		startState[i] = start[i] - '0';
	}

	EightPuzzle game(startState);
	game.Run();

	return 0;
}



