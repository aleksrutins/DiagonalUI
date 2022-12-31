package com.rutins.aleks.diagonalui

import javafx.scene.control.ListCell
import javafx.scene.paint.Color

class TestListCell : ListCell<Test>() {
    override fun updateItem(test: Test?, empty: Boolean) {
        super.updateItem(test, empty)
        if(item == null) return
        text = item.name
        textFill =
            if(item.showProgress) Color.BLACK
            else if(item.showSuccess) Color.GREEN
            else if(item.showFailure) Color.RED
            else Color.GRAY
    }
}