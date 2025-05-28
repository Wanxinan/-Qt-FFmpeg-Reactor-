#ifndef _WD_CONFIGURATION_H_
#define _WD_CONFIGURATION_H_

#include <map>
#include <string>

using namespace std;

namespace wd
{

class Configuration
{
    class AutoRelease
    {
    public:
        AutoRelease(){}
        ~AutoRelease() {
            if(_pInstance) {
                delete _pInstance;
                _pInstance = nullptr;
            }
        }
    };
public:
    static Configuration * getInstance();
	map<string, string> & getConfigMap();
    void display() const;
private:
    Configuration();
    ~Configuration();

	void readConfiguration();
private:
	map<string, string> _configMap;
    static Configuration * _pInstance;
    static AutoRelease _ar;
};


}//end of namespace wd

#endif
