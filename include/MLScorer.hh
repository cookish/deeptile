//
// Created by Martin Cook on 2019-05-02.
//

#ifndef DEEPTILE_MLSCORER_HH
#define DEEPTILE_MLSCORER_HH

#include "ScorerInterface.hh"

#include <string>
using std::string;


class Settings;
class MLScorer : public ScorerInterface
{
public:
    MLScorer(std::shared_ptr<BoardHandler> bh, const Settings* settings, int port);
    ~MLScorer();
    void scoreMapOfBoards(std::unordered_map<Board, double> &map) override;
    double getScoreSpawned(Board board) override;
    double getScoreMoved(Board board) override { return getScoreSpawned(board); }
    double getScoreMovedFast(Board board) override;

private:
    std::shared_ptr<BoardHandler> bh;
    const Settings* settings;
    void sendRequest(const string& request, string& result);
    bool checkAndCleanResponse(string &result);
    int port;
};


#endif //DEEPTILE_MLSCORER_HH
