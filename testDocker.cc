//
// Created by Martin Cook on 2019-05-23.
//

#include <stdio.h>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
namespace nh = nlohmann;

#include <vector>
using std::vector;

#include <iostream>
using std::cout;
using std::endl;

#ifndef WIN32
#define __stdcall
#endif //For compatibility with both Linux and Windows


int main(void)
{
    CURL *curl;
    CURLcode res;

    /* In windows, this will init the winsock stuff */
    curl_global_init(CURL_GLOBAL_ALL);

    /* get a curl handle */
    curl = curl_easy_init();
    vector<vector<vector<vector< int> > > > x = {{{{15}, {14}, {14}, {12}}, {{8}, {9}, {10}, {11}},
                                                  {{7}, {6}, {5}, {4}}, {{0}, {1}, {2}, {3}}}};
    nh::json j;
    j["instances"] = x;
    auto xstr = j.dump();
//    cout << xstr << endl;

    std::string resultBody { };
    if(curl) {
        /* First set the URL that is about to receive our POST. This URL can
           just as well be a https:// URL if that is what should receive the
           data. */
        curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:8503/v1/models/serving_default:predict");
//        curl_easy_setopt(curl, CURLOPT_PORT, "8503");
//        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
        curl_easy_setopt(curl, CURLOPT_POST, 1L);

        /* Now specify the POST data */
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, xstr.c_str());
//               {\"instances\": [[[[15,14,14,12],[8,9,10,11],[7,6,5,4],[0,1,2,3]]]]}
//            "{\"instances\": [[[[15], [14], [13], [12]], [[8], [9], [10], [11]], [[7], [6], [5], [4]], [[0], [1], [2], [3]]]]}");



        // this adapted from answer in
        // https://stackoverflow.com/questions/9786150/save-curl-content-result-into-a-string-in-c/9786295#9786295
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &resultBody);
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
    cout << resultBody << endl;
    curl_global_cleanup();
    return 0;
}