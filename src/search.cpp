#include "search.hpp"
#include "eval.hpp"
#include "movegen.hpp"
#include <iostream>
#include <cmath>

HistoryTable Search::historyTable;
KillerTable Search::killerTable;

std::array<int, 2> Search::contempt;

const int Search::aspirationWindow = 50;
const int Search::nullReduction = 3;
const int Search::futilityDepth = 4;
const std::array<int, 1 + 4> Search::futilityMargins = {
    50, 125, 125, 300, 300
};
const int Search::lmrFullDepthMoves = 4;
const int Search::lmrReductionLimit = 3;
std::array<int, 256> Search::lmrReductions;
const int Search::lmpDepth = 4;
const std::array<int, 1 + 4> Search::lmpMargins = {
    0, 4, 8, 16, 32
};

const int16_t Search::hashMove = 32767;
const int16_t Search::captureMove = hashMove - 2000;
const std::array<int16_t, 1 + 4> Search::killerMove = {
    0, captureMove - 2000, captureMove - 2001, captureMove - 2002, captureMove - 2003
};

std::array<Move, 32> Search::pv[32];
std::array<int, 32> Search::pvLength;

void Search::initialize()
{
    contempt.fill(0);
    for (auto i = 0; i < 256; ++i)
    {
        lmrReductions[i] = static_cast<int>(std::max(1.0, std::round(std::log(i + 1))));
    }
}

void Search::orderMoves(const Position & pos, MoveList & moveList, const Move & ttMove, int ply)
{
    for (auto i = 0; i < moveList.size(); ++i)
    {
        auto & move = moveList[i];
        if (move.getMove() == ttMove.getMove()) // Move from transposition table
        {
            move.setScore(hashMove);
        }
        else if (pos.getBoard(move.getTo()) != Piece::Empty 
             || (move.getPromotion() != Piece::Empty && move.getPromotion() != Piece::King))
        {
            auto score = pos.SEE(move);
            if (score >= 0) // Order good captures and promotions after ttMove
            {
                score += captureMove;
            }
            move.setScore(score);
        }
        else
        {
            auto killerScore = killerTable.isKiller(move, ply);
            if (killerScore > 0)
            {
                move.setScore(killerMove[killerScore]);
            }
            else
            {
                move.setScore(historyTable.getScore(pos, move));
            }
        }
    }
}

void Search::orderCaptures(const Position & pos, MoveList & moveList)
{
    for (auto i = 0; i < moveList.size(); ++i)
    {
        moveList[i].setScore(pos.SEE(moveList[i]));
    }
}

void Search::selectMove(MoveList & moveList, int currentMove)
{
    auto bestMove = currentMove;
    auto bestScore = moveList[currentMove].getScore();

    for (auto i = currentMove + 1; i < moveList.size(); ++i)
    {
        if (moveList[i].getScore() > bestScore)
        {
            bestScore = moveList[i].getScore();
            bestMove = i;
        }
    }

    if (bestMove > currentMove)
    {
        std::swap(moveList[currentMove], moveList[bestMove]);
    }
}

int Search::qSearch(Position & pos, int ply, int alpha, int beta, bool inCheck)
{
    int bestScore, delta;
    MoveList moveList;
    History history;

    // delete this
    pvLength[ply] = ply;

    if (inCheck)
    {
        bestScore = matedInPly(ply);
        MoveGen::generateLegalEvasions(pos, moveList);
        // orderMoves
    }
    else
    {
        bestScore = Evaluation::evaluate(pos);
        if (bestScore > alpha)
        {
            if (bestScore >= beta)
            {
                return bestScore;
            }
            alpha = bestScore;
        }
        delta = bestScore + futilityMargins[0];
        MoveGen::generatePseudoLegalCaptureMoves(pos, moveList);
        orderCaptures(pos, moveList);
    }

    for (auto i = 0; i < moveList.size(); ++i)
    {
        selectMove(moveList, i);
        const auto & move = moveList[i];

        if (!inCheck) // don't do any pruning if in check
        {
            // Bad capture pruning + delta pruning. Assumes that the moves are sorted from highest SEE value to lowest.
            if (move.getScore() < 0 || (delta + move.getScore() < alpha))
            {
                break;
            }
        }

        if (!pos.makeMove(move, history))
        {
            continue;
        }

        auto score = -qSearch(pos, ply + 1, -beta, -alpha, pos.inCheck());
        pos.unmakeMove(move, history);

        if (score > bestScore)
        {
            if (score > alpha)
            {
                if (score >= beta)
                {
                    return score;
                }
                alpha = score;

                // delete this
                pv[ply][ply] = move;
                for (auto i = ply + 1; i < pvLength[ply + 1]; ++i)
                {
                    pv[ply][i] = pv[ply + 1][i];
                }
                pvLength[ply] = pvLength[ply + 1];
            }
            bestScore = score;
        }
    }

    return bestScore;
}



