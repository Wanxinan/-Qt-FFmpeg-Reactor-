#include <iostream>
#include <string>
#include <unordered_map>
#include <curl/curl.h>
#include "json.hpp" // 包含nlohmann/json头文件

using namespace std;
using json = nlohmann::json;

class HttpClient {
public:
    HttpClient() {
        curl_global_init(CURL_GLOBAL_DEFAULT);
        cout << "HttpClient()" << endl;
    }

    ~HttpClient() {
        curl_global_cleanup();
    }

    string get(const string &url) {
        CURL *curl = curl_easy_init();
        string response;

        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

            CURLcode res = curl_easy_perform(curl);
            if (res != CURLE_OK) {
                cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << endl;
            }

            curl_easy_cleanup(curl);
        }

        return response;
    }

private:
    static size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
        size_t total_size = size * nmemb;
        string *response = static_cast<string*>(userp);
        response->append(static_cast<char*>(contents), total_size);
        return total_size;
    }
public:
    unordered_map<string, string> parse_json_to_string_map(const string &json_str) {
        unordered_map<string, string> result;
        try {
            json j = json::parse(json_str);
            // 遍历JSON对象的每个键值对
            for (auto it = j.begin(); it != j.end(); ++it) {
                // 将JSON值转换为字符串
                result[it.key()] = it.value().dump();
            }
        } catch (const json::parse_error &e) {
            cerr << "JSON parse error: " << e.what() << endl;
        }
        return result;
    }
};
