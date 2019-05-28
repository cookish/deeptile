//
// Created by Martin Cook on 2019-05-02.
//

#include "MLScorer.hh"
#include "Settings.hh"

#include <curl/curl.h>
#include <sstream>
#include <array>
using std::array;

#include <iostream>
using std::cout;
using std::endl;


#ifndef WIN32
#define __stdcall
#endif //For compatibility with both Linux and Windows


MLScorer::MLScorer(std::shared_ptr<BoardHandler> bh, const Settings *settings, int port)
    : bh(std::move(bh)), settings(settings), port(port)
{}

MLScorer::~MLScorer() = default;

void MLScorer::scoreMapOfBoards(std::unordered_map<Board, double> &map) {
    vector<Board> boardsToScore;
    double placeHolderAlive = settings->score_for_death + 1.;
    size_t numBoardsToQuery = 0;

    string json = "{\"instances\":[";
    string seperator{};
    for (auto &boardProb: map) {
        if (bh->isDead(boardProb.first)) {
            boardProb.second = settings->score_for_death;
        } else {
            boardProb.second = placeHolderAlive;
            json += seperator + "[" + BoardHandler::toJson(boardProb.first) + "]";
            seperator = ",";
            ++numBoardsToQuery;
        }
    }
    json += "]}";
    string result;
    if (numBoardsToQuery > 0) {
        sendRequest(json, result);
        if (!checkAndCleanResponse(result)) {
            cout << "Warning: JSON result not in expected format. Skipping." << endl;
        }
    }

    // update map
    std::stringstream resultStream{result};
    double prediction;

    for (auto &mapItem : map) {
        if (mapItem.second != settings->score_for_death) { // dead
            bool readSuccess{resultStream >> prediction};
            if (!readSuccess) {
                cout << "Warning! Mismatch between number of predictions and number of boards" << endl;
                break;
            }
            mapItem.second = prediction;
        }
    }
    // should be no more numbers left
    bool readSuccess{resultStream >> prediction};
    if (readSuccess) {
        cout << "Warning! Mismatch between number of predictions and number of boards" << endl;
    }
}


double MLScorer::getScoreSpawned(Board) {
    // not yet implemented
    return -1.;
}

double MLScorer::getScoreMovedFast(Board board) {
    return static_cast<double>(board);
}

void MLScorer::sendRequest(const string &request, string &result) {
    CURL *curl;
    CURLcode res;

    /* In windows, this will init the winsock stuff */
    curl_global_init(CURL_GLOBAL_ALL);

    /* get a curl handle */
    curl = curl_easy_init();

    result = "";
    if(curl) {
        /* First set the URL that is about to receive our POST. This URL can
           just as well be a https:// URL if that is what should receive the
           data. */
        string host = "http://localhost:" + std::to_string(port) + "/v1/models/serving_default:predict";
        curl_easy_setopt(curl, CURLOPT_URL, host.c_str());
        curl_easy_setopt(curl, CURLOPT_PORT, port);
//        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        /* Now specify the POST data */
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, request.c_str());

        // this adapted from answer in
        // https://stackoverflow.com/questions/9786150/save-curl-content-result-into-a-string-in-c/9786295#9786295
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, static_cast<size_t (__stdcall *)(char*, size_t, size_t, void*)>(
            [](char* ptr, size_t size, size_t nmemb, void* resultBody){
                *(static_cast<std::string*>(resultBody)) += std::string {ptr, size * nmemb};
                return size * nmemb;
            }));

        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);
        /* Check for errors */
        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));

        /* always cleanup */
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
}

bool MLScorer::checkAndCleanResponse(string &result) {
    //remove whitespace from result
    string whitespace = "\n ";
    for (char c : whitespace) {
        result.erase(std::remove(result.begin(), result.end(), c), result.end());
    }

    string start = "{\"predictions\":";
    if (result.rfind(start, 0) != 0) {
        return false;
    }

    // remove initial tag
    result.erase(0, start.size());

    // remove brackets, to leave comma separated list
    string brackets = "{}[]";
    for (char c : brackets) {
        result.erase(std::remove(result.begin(), result.end(), c), result.end());
    }
    // make space separated list of floats
    std::replace(result.begin(), result.end(), ',', ' ');
    return true;
}
