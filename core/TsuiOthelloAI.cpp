#include "TsuiOthelloAI.hpp"
#include <ics46/factory/DynamicFactory.hpp>
ICS46_DYNAMIC_FACTORY_REGISTER(OthelloAI, tsuiat::TsuiOthelloAI, "Tsui AI (Required)")


const int MAX = 999;
const int MIN = -999;
//check how many corners are occupied by my AI
int tsuiat::TsuiOthelloAI::checkAiCorner(std::unique_ptr<OthelloGameState>& s)
{
	int count = 0;
	if(s->board().cellAt(0,0) == color) count++;
	else if(s->board().cellAt(0,s->board().height()-1) == color) count++;
	else if(s->board().cellAt(s->board().width()-1,0) == color) count++;
	else if(s->board().cellAt(s->board().width()-1,s->board().height()-1) == color) count++;
	return 20*count;
}
//check how many corners are occupied by opponent
int tsuiat::TsuiOthelloAI::checkOppCorner(std::unique_ptr<OthelloGameState>& s)
{
	int count = 0;
	if(s->board().cellAt(0,0) != color) count++;
	else if(s->board().cellAt(0,s->board().height()-1) != color) count++;
	else if(s->board().cellAt(s->board().width()-1,0) != color) count++;
	else if(s->board().cellAt(s->board().width()-1,s->board().height()-1) != color) count++;
	return -20*count;
}
//check how many adjacent corner cells are occupied by my AI
int tsuiat::TsuiOthelloAI::checkOppAdjacent(std::unique_ptr<OthelloGameState>& s)
{
	int count = 0;
	//top left
	if(s->board().cellAt(0,1) != color) count++;
	else if(s->board().cellAt(1,0) != color) count++;
	else if(s->board().cellAt(1,1) != color) count++;
	//top right
	else if(s->board().cellAt(s->board().width()-2,0) != color) count++;
	else if(s->board().cellAt(s->board().width()-2,1) != color) count++;
	else if(s->board().cellAt(s->board().width()-1,1) != color) count++;
	//bottom left
	else if(s->board().cellAt(0,s->board().height()-2) != color) count++;
	else if(s->board().cellAt(1,s->board().height()-2) != color) count++;
	else if(s->board().cellAt(1,s->board().height()-1) != color) count++;
	//bottom right
	else if(s->board().cellAt(s->board().width()-2,s->board().height()-1) != color) count++;
	else if(s->board().cellAt(s->board().width()-2,s->board().height()-2) != color) count++;
	else if(s->board().cellAt(s->board().width()-1,s->board().height()-2) != color) count++;
	return 3*count;
}
//check how many adjacent corner cells are occupied by opponent
int tsuiat::TsuiOthelloAI::checkAiAdjacent(std::unique_ptr<OthelloGameState>& s)
{
	int count = 0;
	//top left
	if(s->board().cellAt(0,1) == color) count++;
	else if(s->board().cellAt(1,0) == color) count++;
	else if(s->board().cellAt(1,1) == color) count++;
	//top right
	else if(s->board().cellAt(s->board().width()-2,0) == color) count++;
	else if(s->board().cellAt(s->board().width()-2,1) == color) count++;
	else if(s->board().cellAt(s->board().width()-1,1) == color) count++;
	//bottom left
	else if(s->board().cellAt(0,s->board().height()-2) == color) count++;
	else if(s->board().cellAt(1,s->board().height()-2) == color) count++;
	else if(s->board().cellAt(1,s->board().height()-1) == color) count++;
	//bottom right
	else if(s->board().cellAt(s->board().width()-2,s->board().height()-1) == color) count++;
	else if(s->board().cellAt(s->board().width()-2,s->board().height()-2) == color) count++;
	else if(s->board().cellAt(s->board().width()-1,s->board().height()-2) == color) count++;
	return -3*count;
}
//evaluation function that determines the score for each end game state
int tsuiat::TsuiOthelloAI::evaluate(std::unique_ptr<OthelloGameState>& s)
{
	return (checkAiCorner(s) + checkOppCorner(s)) + (checkAiAdjacent(s) - checkOppAdjacent(s));
}
//function that returns the valid moves my AI can make
std::vector<std::pair<int, int>> tsuiat::TsuiOthelloAI::validMoves(std::unique_ptr<OthelloGameState>& s)
{
	std::vector<std::pair<int, int>> valid;
	for(int x = 0; x < s->board().width(); ++x)
	{
		for(int y = 0; y < s->board().height(); ++y)
		{
			if(s->board().cellAt(x,y) == OthelloCell::empty && s->isValidMove(x,y))
			{
				valid.push_back(std::pair<int, int>(x,y));
			}
		}
	}
	return valid;
}
//recursive function that works to find the best end game state
int tsuiat::TsuiOthelloAI::search(std::unique_ptr<OthelloGameState>& s, int depth, int alpha, int beta)
{
	std::vector<std::pair<int, int>> valid = validMoves(s);
	if(depth == 0  || s->isGameOver())
		return evaluate(s);
	else
	{
		if((color == OthelloCell::black && s->isBlackTurn()) || (color == OthelloCell::white && s->isWhiteTurn()))
		{
			int best = MIN;
			for(int i = 0; i < valid.size(); ++i)
			{
				std::unique_ptr<OthelloGameState> nextS = s->clone();
				nextS->makeMove(valid[i].first, valid[i].second);
				int value = search(nextS, depth-1, alpha, beta);
				best = std::max(best, value);
				alpha = std::max(best, alpha);
				if(beta <= alpha)
					break;
			}
			return best;
		}
		else
		{
			int best = MAX;
			for(int j = 0; j < valid.size(); ++j)
			{
				std::unique_ptr<OthelloGameState> oppS = s->clone();
				oppS->makeMove(valid[j].first, valid[j].second);
				int value = search(oppS, depth-1, alpha, beta);
				best = std::min(best, value);
				beta = std::min(best, beta);
				if(beta <= alpha)
					break;
			}
			return best;
		}
	}
}
std::pair<int,int> tsuiat::TsuiOthelloAI::chooseMove(const OthelloGameState& state)
{
	//determine AI's color
	if(state.isBlackTurn())
		color = OthelloCell::black;
	else
		color = OthelloCell::white;

	std::pair<int, int> bestPair;
	int value = 0;
	int bestValue = MIN;
	for(int x = 0; x < state.board().width(); ++x)
	{
		for(int y = 0; y < state.board().height(); ++y)
		{
			if(state.board().cellAt(x,y) == OthelloCell::empty && state.isValidMove(x,y))
			{
				std::unique_ptr<OthelloGameState> s = state.clone();
				s->makeMove(x, y);
				value = search(s,4, MIN, MAX);
				if(value > bestValue)
				{
					bestValue = value;
					bestPair.first = x;
					bestPair.second = y;
				}
			}
		}
	}
	return bestPair;
}