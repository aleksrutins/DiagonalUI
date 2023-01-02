#ifndef DIAGONALUI_LOGPARSER_H
#define DIAGONALUI_LOGPARSER_H

#include <string>
#include <QJsonValue>
#include <map>
#include <functional>

namespace DiagonalUI {

class LogParser
{
    std::string tag;
    std::unordered_map<std::string, std::function<void(QJsonValue)>> handlers;
public:
    LogParser(std::string tag) : handlers(), tag(tag) {}
    void addHandler(std::string name, std::function<void(QJsonValue)> handler);
    void processMessage(std::string message);
};

} // namespace DiaogonalUI

#endif // DIAGONALUI_LOGPARSER_H
