#include "EightPuzzle.h"
#include <algorithm>
#include <vector>
#include <cmath>
#include <iostream>

struct Swap_t
{
	unsigned int FirstIndex;
	unsigned int SecondIndex;

	bool operator==(const Swap_t& state)
	{
		return ((FirstIndex == state.SecondIndex && SecondIndex == state.FirstIndex)
			||
			(FirstIndex == state.FirstIndex && SecondIndex == state.SecondIndex));
	}
};

struct Node_t
{
	int State[9];
	Node_t* ParentNode;
	unsigned int CostFromRoot;
	unsigned int TotalCost;
	Swap_t ParentSwap;

	bool operator==(const Node_t& node)
	{
		for (int i = 0; i < 9; i++)
		{
			if (State[i] != node.State[i])
				return false;
		}

		return true;
	}

};



static std::vector<Swap_t> FindAvailableSwaps(const int* state)
{
	unsigned int holeIndex{};
	for (int i = 0; i < 9; i++)
	{
		if (state[i] == 0)
		{
			holeIndex = i;
			break;
		}
	}

	std::vector<Swap_t> availableSwaps;
	switch (holeIndex)
	{
		case 0:
			availableSwaps.push_back({holeIndex, holeIndex +1});
			availableSwaps.push_back({holeIndex, holeIndex +3});
			return availableSwaps;
		case 1:
			availableSwaps.push_back({holeIndex, holeIndex -1});
			availableSwaps.push_back({holeIndex, holeIndex +1});
			availableSwaps.push_back({holeIndex, holeIndex +3});
			return availableSwaps;
		case 2:
			availableSwaps.push_back({holeIndex, holeIndex -1});
			availableSwaps.push_back({holeIndex, holeIndex +3});
			return availableSwaps;
		case 3:
			availableSwaps.push_back({holeIndex, holeIndex -3});
			availableSwaps.push_back({holeIndex, holeIndex +1});
			availableSwaps.push_back({holeIndex, holeIndex +3});
			return availableSwaps;
		case 4:
			availableSwaps.push_back({holeIndex, holeIndex -3});
			availableSwaps.push_back({holeIndex, holeIndex -1});
			availableSwaps.push_back({holeIndex, holeIndex +1});
			availableSwaps.push_back({holeIndex, holeIndex +3});
			return availableSwaps;
		case 5:
			availableSwaps.push_back({holeIndex, holeIndex -3});
			availableSwaps.push_back({holeIndex, holeIndex -1});
			availableSwaps.push_back({holeIndex, holeIndex +3});
			return availableSwaps;
		case 6:
			availableSwaps.push_back({holeIndex, holeIndex -3});
			availableSwaps.push_back({holeIndex, holeIndex +1});
			return availableSwaps;
		case 7:
			availableSwaps.push_back({holeIndex, holeIndex -3});
			availableSwaps.push_back({holeIndex, holeIndex -1});
			availableSwaps.push_back({holeIndex, holeIndex +1});
			return availableSwaps;
		case 8:
			availableSwaps.push_back({holeIndex, holeIndex -3});
			availableSwaps.push_back({holeIndex, holeIndex -1});
			return availableSwaps;
	}
}

// refrence : https://www.geeksforgeeks.org/check-instance-8-puzzle-solvable/
//////////////////////////////////////////////////////////////////////////////
static int getInvCount(int arr[])
{
	int inv_count = 0;
	for (int i = 0; i < 9 - 1; i++)
		for (int j = i + 1; j < 9; j++)
			// Value 0 is used for empty space
			if (arr[j] && arr[i] && arr[i] > arr[j])
				inv_count++;
	return inv_count;
}

// This function returns true if given 8 puzzle is solvable.
static bool isSolvable(int puzzle[9])
{
	// Count inversions in given 8 puzzle
	int invCount = getInvCount(puzzle);

	// return true if inversion count is even.
	return (invCount % 2 == 0);
}
//////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

void EightPuzzle::Run()
{
	if (isSolvable(m_Start))
		SolveIt();
	else
		std::cout << "this start state is not solvable!!!!" << std::endl;
}

EightPuzzle::EightPuzzle(int startState[9])
{
	int goal[] = {
		1,2,3,
		4,5,6,
		7,8,0
	};

	for (int i = 0; i < 9; i++)
	{
		m_Start[i] = startState[i];
	}

	std::copy(std::begin(goal), std::end(goal), std::begin(m_Goal));
}

static void Swap(int* state, Swap_t swap)
{
	int temp = state[swap.FirstIndex];
	state[swap.FirstIndex] = state[swap.SecondIndex];
	state[swap.SecondIndex] = temp;
}
struct less_t
{
	bool operator()(const Node_t* first,const Node_t* second)
	{
		return ( first->TotalCost < second->TotalCost);
	}
} less;

static bool EqualStates(const int* first, const int* second)
{
	for (int i = 0; i < 9; i++)
	{
		if (first[i] != second[i])
			return false;
	}
	return true;
}

static void PrintState(int* state)
{
	for (int i = 1; i <= 9; i++)
	{
		std::cout << state[i - 1] << "\t";

		if (i % 3 == 0)
			std::cout << std::endl;
	}

	std::cout << std::endl;
	std::cout << "----------------------------";
	std::cout << std::endl;
	std::cout << std::endl;
}

void EightPuzzle::SolveIt()
{
	// Initialize the root
	Node_t* root = new Node_t;
	root->ParentNode = nullptr;
	std::copy(std::begin(m_Start), std::end(m_Start), std::begin(root->State));
	root->CostFromRoot = 0;
	root->TotalCost = CalcHeuristic(root->State);

	// Initialize the open and closed list
	// open -> priority queue
	// closed -> vector
	std::vector<Node_t*> openList;
	std::vector<Node_t*> closedList; // holds all expandded nodes
	
	// Add the Root to the openList
	openList.push_back(root);

	// Iterate over the open list to expand the top node with lowest total cost
	while (openList.size() > 0)
	{
		std::sort(openList.begin(), openList.end(), less);
		auto current = openList[0];
		// if the goal node terminate
		auto heuristic = current->TotalCost - current->CostFromRoot;
		
		if (heuristic == 0) // the top is the goal
			break;

		// Get available swaps for each one we will construct a new node 
		auto availableSwaps = FindAvailableSwaps(current->State);

		for (Swap_t swap : availableSwaps)
		{
			// exclude the parent swap which lead to the current node
			if (swap == current->ParentSwap)
				continue;

			
			// get the new state and get the cost from the root
			int newState[9];
			std::copy(std::begin(current->State), std::end(current->State), std::begin((newState)));
			Swap(newState, swap);
			// now we have the new state
			unsigned int newNodeCostFromRoot = current->CostFromRoot + 1;
			unsigned int newNodeHeuristic;
			// and we have th cost
			Node_t* newNode;

			// check if theere is a node in the closed list with the same state
			bool foundInClosed = false;
			Node_t* closedNode;
			int inClosedndex = -1;
			std::for_each(closedList.begin(), closedList.end(),
				[&foundInClosed, &inClosedndex, &closedNode, newState](Node_t* node)
				{
					inClosedndex++;
					if (EqualStates(node->State, newState))
					{
						foundInClosed = true;
						closedNode = node;
					}
				});

			// check if the node in the open list using the new state
			bool foundInOpened = false;
			Node_t* openedNode;
			std::for_each(closedList.begin(), closedList.end(),
				[&foundInOpened, &openedNode, newState](Node_t* node)
				{
					if (EqualStates(node->State, newState))
					{
						foundInOpened = true;
						openedNode = node;
					}
				});
		
			if (foundInClosed)
			{
				// we have found closed node with the same state
				
				// if we did not find a shorter route -> skip
				if (closedNode->CostFromRoot <= newNodeCostFromRoot)
					continue;

				// other wise remove it from the closed list
				// because we found a closed node with shorter route
				// and we will create a new node with the new cost and add it to the opan list
				closedList.erase(closedList.begin() + inClosedndex);
				
				// we will store this node in newNode and we will update it later and add it to the open list 
				// beacuse it has better route
				newNode = closedNode;

				// we can find the heuristic of the new state
				newNodeHeuristic = closedNode->TotalCost - closedNode->CostFromRoot;
			}

			else if (foundInOpened)
			{
				// skip if we foun that the new node is alridy in the open list and there is no better route
				if (openedNode->CostFromRoot <= newNodeCostFromRoot)
					continue;

				// we will store this node in newNode and we will update it later
				// beacuse it has better route
				newNode = openedNode;

				// we can find the heuristic of the new state
				newNodeHeuristic = closedNode->TotalCost - closedNode->CostFromRoot;
			}

			else
			{
				// we have an unvisted node construct the node and add it to the open list
				newNode = new Node_t;
				// store the new state in the new node
				std::copy(std::begin(newState), std::end(newState), std::begin((newNode->State)));
				// calculate the heuristic
				newNodeHeuristic = CalcHeuristic(newState);
			}
			
			// at this step newNode may be
			// unvisted node with it's new state
			// a closed node with better route -> put it in open list but after some updates
			// an opened node with better route -> update it

			newNode->ParentNode = current;
			newNode->ParentSwap = swap;
			newNode->CostFromRoot = newNodeCostFromRoot;
			newNode->TotalCost = newNodeCostFromRoot + newNodeHeuristic;

			if (!foundInOpened)
			{
				// means that it was a closed node or an unvistied one
				openList.push_back(newNode);
			}


		}
		// we hve finished looking for all new nodes from the current node 
		// time to remove it from the open list and add it to the closed list

		openList.erase(openList.begin());
		closedList.push_back(current);
	}

	// we have reached the goal or there is no goal
	// we will print the optimal path
	Node_t* goal = openList[0];
	std::vector<Node_t*> optimalPath;
	
	while (true)
	{
		optimalPath.push_back(goal);
		goal = goal->ParentNode;
		if (goal == nullptr)
			break;
	}
	
	std::reverse(optimalPath.begin(), optimalPath.end());

	for (auto state : optimalPath)
	{
		PrintState(state->State);
	}
	
	std::cout << "Path Depth: " << openList[0]->CostFromRoot << std::endl;
	std::cout << "Nodes count : " << openList.size() + closedList.size() << std::endl;
	std::cout << "Open Nodes count : " << openList.size() << std::endl;
	std::cout << "Closed Nodes count : " << closedList.size() << std::endl;

	for (auto node : closedList)
	{
		delete node;
	}

	for (auto node : openList)
	{
		delete node;
	}

}



unsigned int EightPuzzle::CalcHeuristic(const int* state)
{
	unsigned int heuristic = 0;

	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (m_Goal[i] == state[j])
			{
				heuristic += std::abs(j - i);
			}
		}
	}
	
	/*for (int i = 0; i < 9; i++)
	{
		if (state[i] != m_Goal[i])
			heuristic++;
	}*/
	return heuristic;
}

