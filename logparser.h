#ifndef DIAGONALUI_LOGPARSER_H
#define DIAGONALUI_LOGPARSER_H

#include <QJsonValue>
#include <QHash>
#include <QString>
#include <functional>
#include <QJsonObject>
#include <QJsonArray>

namespace DiagonalUI {

class LogParser
{
    QString tag;
    QHash<QString, std::function<void(QJsonValue)>> handlers;
public:
    LogParser(QString tag) : tag(tag), handlers() {}
    void addValueHandler(QString name, std::function<void(QJsonValue)> handler);
    template <typename T>
    void addHandler(QString name, std::function<void(T)> handler) {
        addValueHandler(name, createHandler<T>()(handler));
    };
    void processMessage(QString message);
private:
    template <typename T>
    constexpr std::function<std::function<void(QJsonValue)>(std::function<void(T)>)> createHandler() {
        if constexpr(std::is_same<T, QString>::value) {
            return [](std::function<void(QString)> handler) { return [=] (QJsonValue value) { handler(value.toString()); }; };
        } else if constexpr(std::is_same<T, double>::value) {
            return [](std::function<void(double)> handler) { return [=] (QJsonValue value) { handler(value.toDouble()); }; };
        } else if constexpr(std::is_same<T, bool>::value) {
            return [](std::function<void(bool)> handler) { return [=] (QJsonValue value) { handler(value.toBool()); }; };
        } else if constexpr(std::is_same<T, int>::value) {
            return [](std::function<void(int)> handler) { return [=] (QJsonValue value) { handler(value.toInt()); }; };
        } else if constexpr(std::is_same<T, QJsonObject>::value) {
            return [](std::function<void(QJsonObject)> handler) { return [=] (QJsonValue value) { handler(value.toObject()); }; };
        } else if constexpr(std::is_same<T, QJsonArray>::value) {
            return [](std::function<void(QJsonArray)> handler) { return [=] (QJsonValue value) { handler(value.toArray()); }; };
        } else if constexpr(std::is_same<T, QJsonValue>::value) {
            return [](std::function<void(QJsonValue)> handler) { return [=] (QJsonValue value) { handler(value); }; };
        } else {
            return [](auto){ return [](auto){}; };
        }
    }
};

} // namespace DiaogonalUI

#endif // DIAGONALUI_LOGPARSER_H
