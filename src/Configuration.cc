#include "Configuration.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iostream>

namespace wd
{

//使用饿汉模式
Configuration * Configuration::_pInstance = getInstance();
Configuration::AutoRelease Configuration::_ar;

Configuration * Configuration::getInstance()
{
    if(nullptr == _pInstance) {
        _pInstance = new Configuration();
    }
    return _pInstance;
}

Configuration::Configuration()
{
    cout << "Configuration()" << endl;
    readConfiguration();
}

Configuration::~Configuration()
{   cout << "~Configuration()" << endl; }

void Configuration::readConfiguration()
{
    string configfile("./conf/server.conf");
	ifstream ifs(configfile);
	if(ifs.good())
	{
		string line;
		while(getline(ifs, line))
		{
			istringstream iss(line);
			string key;
			string value;
			iss >> key >> value;
			_configMap[key] = value;
		}
        //重要的节点做个日志记录
        //比如这里配置文件读取完毕
        cout << "reading config file " << configfile << " is over" << endl;
	}
	else
	{
        cout << "reading config file " << configfile << " error" << endl;
	}
}

map<string, string> & Configuration::getConfigMap()
{
	return _configMap;
}

void Configuration::display() const
{
    for(auto & elem : _configMap) {
        cout << elem.first << " ---> " << elem.second << endl;
    }
    cout << endl;
}

}// end of namespace wd
