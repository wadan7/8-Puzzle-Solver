#pragma once


class EightPuzzle
{
public:
	void Run();
	EightPuzzle(int startState[9]);
	void SolveIt();

	const int* GetStart() const { return m_Start; };
	const int* GetGoal() const { return m_Goal; };

private:
	//void onStart();
	unsigned int CalcHeuristic(const int* state);


private:
	int m_Start[9];
	int m_Goal[9];
	bool m_ShoudClose;

};