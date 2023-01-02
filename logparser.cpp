#include "logparser.h"

namespace DiagonalUI {

void LogParser::addHandler(std::string name, std::function<void(QJsonValue)> handler) {
    this->handlers[name] = handler;
}

void LogParser::processMessage(std::string message) {
    auto tagComment = "/* log:tag:" + this->tag + " */";
    auto index = message.find(tagComment);
    if(index == -1) return;
    index = index + tagComment.length(); // find actual start of JSON
}

} // namespace DiagonalUI
