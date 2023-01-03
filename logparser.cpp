#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QStringRef>
#include "logparser.h"

namespace DiagonalUI {

void LogParser::addValueHandler(QString name, std::function<void(QJsonValue)> handler) {
    this->handlers[name] = handler;
}

void LogParser::processMessage(QString message) {
    auto tagComment = "/* log:tag:" + this->tag + " */";
    auto index = message.indexOf(tagComment);
    if(index == -1) return;
    index = index + tagComment.length(); // find actual start of JSON
    auto jsonStr = QStringRef(&message, index, (message.length() - index));
    QJsonObject jsonObj = QJsonDocument::fromJson(jsonStr.toUtf8()).object();
    if(jsonObj["message"].isString() && this->handlers.contains(jsonObj["message"].toString())) {
        auto handler = this->handlers[jsonObj["message"].toString()];
        handler(jsonObj["data"]);
    }
}

} // namespace DiagonalUI
