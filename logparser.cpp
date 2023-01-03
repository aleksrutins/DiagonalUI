#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "logparser.h"
using namespace std;

namespace DiagonalUI {

void LogParser::addValueHandler(QString name, std::function<void(QJsonValue)> handler) {
    this->handlers[name] = handler;
}

void LogParser::processMessage(QString message) {
    auto tagComment = "/* log:tag:" + this->tag + " */";
    auto index = message.indexOf(tagComment);
    if(index == -1) return;
    index = index + tagComment.length(); // find actual start of JSON
    auto jsonStr = QString(message.data() + index);
    QJsonObject jsonObj = QJsonDocument::fromJson(jsonStr.toUtf8()).object();
    if(jsonObj["Message"].isString() && this->handlers.contains(jsonObj["Message"].toString())) {
        auto handler = this->handlers[jsonObj["Message"].toString()];
        handler(jsonObj["Data"]);
    }
}

} // namespace DiagonalUI
