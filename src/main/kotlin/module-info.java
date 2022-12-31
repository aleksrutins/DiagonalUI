module com.rutins.aleks.diagonalui {
    requires javafx.controls;
    requires javafx.fxml;
    requires kotlin.stdlib;
    requires kotlinx.serialization.core;
    requires kotlinx.serialization.json;


    opens com.rutins.aleks.diagonalui to javafx.fxml;
    exports com.rutins.aleks.diagonalui;
}