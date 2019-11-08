#ifndef TSUIOTHELLOAI_HPP
#define TSUIOTHELLOAI_HPP
#include "OthelloAI.hpp"
#include <vector>
#include <utility>
#include <algorithm>
#include <iostream>
namespace tsuiat
{
class TsuiOthelloAI: public OthelloAI
{
private:
	OthelloCell color;
public:
	virtual int checkAiCorner(std::unique_ptr<OthelloGameState>& s);
	virtual int checkOppCorner(std::unique_ptr<OthelloGameState>& s);
	virtual int checkAiAdjacent(std::unique_ptr<OthelloGameState>& s);
	virtual int checkOppAdjacent(std::unique_ptr<OthelloGameState>& s);
	virtual int evaluate(std::unique_ptr<OthelloGameState>& s);
	virtual std::vector<std::pair<int, int>> validMoves(std::unique_ptr<OthelloGameState>& s);
	virtual int search(std::unique_ptr<OthelloGameState>& s, int depth, int alpha, int beta);
    virtual std::pair<int, int> chooseMove(const OthelloGameState& state);
};
}
#endif