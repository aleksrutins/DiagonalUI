package com.rutins.aleks.diagonalui

import kotlinx.serialization.decodeFromString
import kotlinx.serialization.json.Json
import kotlinx.serialization.json.JsonElement

class LogParser(private val tag: String) {
    data class Line(val message: String, val data: JsonElement)

    private val handlers = HashMap<String, (JsonElement) -> Unit>()

    fun addHandler(messageName: String, handler: (JsonElement) -> Unit) {
        handlers.put(messageName, handler)
    }

    fun processLine(line: String) {
        val tagComment = "/* log:tag:$tag */"
        val index = line.indexOf(tagComment)
        if(index != -1) {
            val messageData = line.substring(index + tagComment.length)
            try {
                val deserialized = Json.decodeFromString<Line>(messageData)
                if (deserialized.message in handlers) {
                    handlers[deserialized.message]?.invoke(deserialized.data)
                }
            } catch (_: Throwable) {

            }
        }
    }
}