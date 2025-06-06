#include "CryptString.h"
#include <QCryptographicHash>

#include <QByteArray>
#include <QDateTime>
#include <QString>

CryptString::CryptString(const QString & data)
    : _data(data)
{
}

QString CryptString::generateMD5() const
{
    QByteArray arr = QCryptographicHash::hash(_data.toUtf8(), QCryptographicHash::Md5);
    return arr.toHex();
}

KVQuery::KVQuery()
{
    //因为每个请求都需要通过密钥secret和当前时间t来生成token
    //在KVQuery的构造函数中直接加入这两个字段
    _map.insert(std::make_pair("secret", "f6fdffe48c908deb0f4c3bd36c032e72"));
    QDateTime currentDateTime = QDateTime::currentDateTime();
    string secstr = std::to_string(currentDateTime.toSecsSinceEpoch());
    _map.insert(std::make_pair("t", secstr));
}

void KVQuery::add(const std::string &key, const std::string &value)
{
    _map.insert(std::make_pair(key, value));
}

void KVQuery::erase(const std::string &key)
{
    _map.erase(key);
}

void KVQuery::clear()
{
    _map.clear();
}

string KVQuery::toString() const
{   //转换为url中查询词的格式
    string tmp;
    for(auto & elem : _map) {
        tmp.append(elem.first).append("=").append(elem.second).append("&");
    }
    tmp.pop_back();
    return tmp;
}

string KVQuery::toCrpytString()
{
    CryptString cryptStr(QString::fromStdString(toString()));
    QString token = cryptStr.generateMD5();
    this->add("token", token.toStdString());
    //正式生成的url中是不携带secret字段的，因此需要删除
    this->erase("secret");//去掉secret
    return toString();
}
