

/*
This is a UI file (.ui.qml) that is intended to be edited in Qt Design Studio only.
It is supposed to be strictly declarative and only uses a subset of QML. If you edit
this file manually, you might introduce QML code that is not supported by Qt Design Studio.
Check out https://doc.qt.io/qtcreator/creator-quick-ui-forms.html for details on .ui.qml files.
*/
import QtQuick
import QtQuick.Controls
import Backup
import Qt.labs.qmlmodels

Rectangle {
    id: restore
    width: parent.width
    height: parent.height - 40

    color: Constants.backgroundColor

    ScrollView {
        x: parent.width * 0.05
        y: 0
        width: parent.width * 0.9
        height: parent.height * 0.6
        clip: true
        TableView {
            id: tableViewId
            anchors.fill: parent
            clip: true
            boundsBehavior: Flickable.StopAtBounds

            model: toptablemodel

            delegate: Rectangle {
                // implicitWidth: tableViewId.width / 7
                implicitHeight: 50
                color: (heading) ? "grey" : "white"

                Text {
                    text: tabledata
                    font.pointSize: 12
                    anchors.centerIn: parent
                }
            }

            columnWidthProvider: function (column) {
                switch (column) {
                case 0:
                    return tableViewId.width * 0.05;
                case 1:
                    return tableViewId.width * 0.15;
                case 2:
                    return tableViewId.width * 0.3;
                case 3:
                    return tableViewId.width * 0.3;
                default:
                    return tableViewId.width * 0.1;
                }
            }
        }
    }

    Row {
        id: update_row
        y: parent.height * 0.65
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 10

        Text {
            id: delete_row_text
            text: qsTr("删除记录")
            anchors.verticalCenter: parent.verticalCenter
            font.pointSize: 11
        }

        TextField {
            id: delete_row_textField
            objectName: "delete_row_textField"
            width: restore.width * 0.1
            placeholderText: qsTr("输入编号，默认为0")
        }

        Button {
            id: delete_row_button
            text: qsTr("删除")

            Connections {
                target: delete_row_button
                onClicked: {
                    mainclass.delete_from_db()
                    mainclass.show_table()
                }
            }
        }

        Text {
            id: add_row_text
            text: qsTr("添加记录")
            anchors.verticalCenter: parent.verticalCenter
            font.pointSize: 11
        }

        TextField {
            id: add_row_textField
            objectName: "add_row_textField"
            width: restore.width * 0.5
            placeholderText: qsTr("输入文件的相对路径（不以“/”开头）或绝对路径（以“/”开头）")
        }

        Button {
            id: add_row_button
            text: qsTr("添加")

            Connections {
                target: add_row_button
                onClicked: {
                    mainclass.add_to_db()
                    mainclass.show_table()
                }
            }
        }
    }

    Row {
        id: row
        y: parent.height * 0.70
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 10

        Text {
            id: id_restore_text
            text: qsTr("编号")
            anchors.verticalCenter: parent.verticalCenter
            font.pointSize: 11
        }

        TextField {
            id: id_restore_textField
            objectName: "id_restore_textField"
            width: restore.width * 0.1
            placeholderText: qsTr("默认为0")
        }

        Text {
            id: path_restore_text
            text: qsTr("还原到")
            anchors.verticalCenter: parent.verticalCenter
            font.pointSize: 11
        }

        TextField {
            id: path_restore_textField
            objectName: "path_restore_textField"
            width: restore.width * 0.5
            placeholderText: qsTr("输入还原路径的相对路径（不以“/”开头）或绝对路径（以“/”开头）")
        }

        Text {
            id: key_restore_text
            text: qsTr("密码")
            anchors.verticalCenter: parent.verticalCenter
            font.pointSize: 11
        }

        TextField {
            id: key_restore_textField
            objectName: "key_restore_textField"
            width: restore.width * 0.1
            placeholderText: qsTr("输入密码，默认“123456”")
        }

        Button {
            id: restore_button
            text: qsTr("还原")

            Connections {
                target: restore_button
                onClicked: {
                    mainclass.do_restore()
                    mainclass.show_table()
                }
            }
        }
    }

    ScrollView {
        id: restore_logs_scrollView
        x: parent.width * 0.1
        y: parent.height * 0.75
        width: parent.width * 0.8
        height: parent.height * 0.2

        Rectangle {
            color: "white"
            anchors.fill: parent
        }

        ListView {
            id: restore_logs_listView
            x: 0
            y: 0
            width: parent.width
            height: parent.height
            model: restore_logs
            delegate: Text {
                text: model.display
                font.pixelSize: Constants.logFont
            }
        }
    }
}
