package com.rutins.aleks.diagonalui

import javafx.fxml.FXML
import javafx.scene.control.Button
import javafx.scene.control.Label
import javafx.scene.control.ListView
import javafx.scene.control.TextArea
import javafx.scene.control.TextField
import javafx.scene.layout.VBox
import kotlinx.serialization.json.boolean
import kotlinx.serialization.json.jsonObject
import kotlinx.serialization.json.jsonPrimitive
import java.io.BufferedReader
import java.io.InputStreamReader

data class Test(var name: String, var isRunning: Boolean, var hasRun: Boolean, var success: Boolean) {
    val showSuccess
        get() = hasRun && success

    val showFailure
        get() = hasRun && !success

    val showProgress
        get() = !hasRun

    fun notifyStart() { isRunning = true }
    fun statusReport(success: Boolean) {
        isRunning = false
        hasRun = true
        this.success = success
    }
}

class HelloController {
    @FXML
    private lateinit var outputLines: TextArea
    @FXML
    private lateinit var adbPath: TextField
    @FXML
    private lateinit var startLogcatButton: Button
    @FXML
    private lateinit var testList: ListView<Test>

    private val logParser = LogParser("diagonal")

    @FXML
    fun initialize() {
        testList.setCellFactory { TestListCell() }

        logParser.addHandler("logLine") {
            outputLines.text += it.jsonPrimitive.content + "\n"
        }
        logParser.addHandler("clearTests") {
            testList.items.clear()
        }
        logParser.addHandler("enumerateTest") {
            testList.items.add(Test(it.jsonPrimitive.content, false, false, false))
        }
        logParser.addHandler("testStart") { testName ->
            outputLines.text += "*** TEST START: $testName ***"
            testList.items.first { it.name == testName.jsonPrimitive.content }.notifyStart()
        }
        logParser.addHandler("statusReport") { info ->
            outputLines.text += "*** TEST FINISHED: ${info.jsonObject["name"]!!.jsonPrimitive.content} | Status: ${if(info.jsonObject["success"]!!.jsonPrimitive.boolean) "SUCCESS" else "FAILURE"}"
            testList.items.first { it.name == info.jsonObject["name"]!!.jsonPrimitive.content }.statusReport(info.jsonObject["success"]!!.jsonPrimitive.boolean)
        }
    }

    @FXML
    private fun onLogcatButtonClick() {
        adbPath.isDisable = true
        startLogcatButton.isDisable = true
        val pb = ProcessBuilder(if(adbPath.text != "") adbPath.text else "adb", "logcat")
        pb.redirectOutput(ProcessBuilder.Redirect.PIPE)
        val proc = pb.start()
        BufferedReader(InputStreamReader(proc.inputStream)).use { reader ->
            reader.lineSequence().forEach(logParser::processLine)
        }
    }
}