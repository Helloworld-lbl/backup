

/*
This is a UI file (.ui.qml) that is intended to be edited in Qt Design Studio only.
It is supposed to be strictly declarative and only uses a subset of QML. If you edit
this file manually, you might introduce QML code that is not supported by Qt Design Studio.
Check out https://doc.qt.io/qtcreator/creator-quick-ui-forms.html for details on .ui.qml files.
*/
import QtQuick
import QtQuick.Controls
import Backup

Rectangle {
    id: config
    width: parent.width
    height: parent.height - 40

    color: Constants.backgroundColor

    Row {
        id: config_dbPath_row
        width: 494
        spacing: 10
        anchors.horizontalCenter: parent.horizontalCenter

        Text {
            id: config_dbPath_text
            text: qsTr("数据库路径")
            anchors.verticalCenter: parent.verticalCenter
            font.pointSize: 11
        }

        TextField {
            id: config_dbField_textField
            objectName: "config_dbField_textField"
            width: 400
            placeholderText: db_path
        }
    }

    Button {
        id: db_config_button
        objectName: "db_config_button"
        y: 200
        text: qsTr("确定")
        anchors.horizontalCenter: parent.horizontalCenter

        Connections {
            target: db_config_button
            onClicked: {
                mainclass.set_db_path()
            }
        }
    }
}
