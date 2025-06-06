#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>

using namespace std;

class Configuration {

public:

    static Configuration & getInstance() {
        static Configuration instance; 
        return instance;
    }

    string getConfigValue(const string& key) const {
        auto it = config.find(key);
        if (it != config.end()) {
            return it->second;
        }
        return ""; 
    }

    void printConfig() const {
        for (const auto& pair : config) {
            cout << pair.first << " = " << pair.second << endl;
        }
    }
private:
    Configuration() {
        loadConfig("./conf/server.conf");
    }

    Configuration(const Configuration&) = delete;
    Configuration& operator=(const Configuration&) = delete;

    unordered_map<string, string> config;

    void loadConfig(const string& filename) {
        ifstream file(filename);
        string line;

        while (getline(file, line)) {
            size_t pos = line.find('=');
            if (pos != string::npos) {
                string key = line.substr(0, pos);
                string value = line.substr(pos + 1);
                config[key] = value;
            }
        }

        file.close();
    }
};

#if 0
int main() {
    Configuration & config = Configuration::getInstance();

    config.printConfig();

    cout << "IP: " << config.getConfigValue("ip") << endl;
    cout << "Port: " << config.getConfigValue("port") << endl;

    return 0;
}
#endif
