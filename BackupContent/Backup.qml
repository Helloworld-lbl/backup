

/*
This is a UI file (.ui.qml) that is intended to be edited in Qt Design Studio only.
It is supposed to be strictly declarative and only uses a subset of QML. If you edit
this file manually, you might introduce QML code that is not supported by Qt Design Studio.
Check out https://doc.qt.io/qtcreator/creator-quick-ui-forms.html for details on .ui.qml files.
*/
import QtQuick
import QtQuick.Controls
import Backup
import QtQuick.Dialogs

Rectangle {
    id: backup

    width: parent.width
    height: parent.height - 40
    color: Constants.backgroundColor
    property alias fileType_rowWidth: fileType_row.width

    Column {
        id: doBackup_column
        x: parent.width * 0.5
        y: 0
        width: parent.width * 0.5
        height: parent.height
        spacing: 10

        Text {
            id: packup_text
            text: qsTr("备份")
            font.pointSize: 20
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Row {
            id: path_row
            spacing: 10

            Text {
                id: path_text
                text: qsTr("原 路 径 ")
                anchors.verticalCenter: parent.verticalCenter
                font.pointSize: 11
            }

            TextField {
                id: path_textField
                objectName: "path_textField"
                width: doBackup_column.width * 0.9
                placeholderText: qsTr("输入待备份文件（夹）的相对路径（不以“/”开头）或绝对路径（以“/”开头）")
            }
        }

        // Row {
        //     id: pack_row
        //     spacing: 10

        //     Text {
        //         id: pack_text
        //         text: qsTr("打包算法")
        //         anchors.verticalCenter: parent.verticalCenter
        //         font.pointSize: 11
        //     }

        //     ComboBox {
        //         id: pack_comboBox
        //         objectName: "pack_comboBox"
        //         model: ["Tar", "My"]
        //     }
        // }
        Row {
            id: bak_path_row
            spacing: 10

            Text {
                id: bak_path_text
                text: qsTr("备份路径")
                anchors.verticalCenter: parent.verticalCenter
                font.pointSize: 11
            }

            TextField {
                id: bak_path_textField
                objectName: "bak_path_textField"
                width: doBackup_column.width * 0.9
                placeholderText: qsTr("输入备份文件（夹）的相对路径（不以“/”开头）或绝对路径（以“/”开头）")
            }
        }

        Row {
            id: compression_row
            spacing: 10

            CheckBox {
                id: compression_checkBox
                objectName: "compression_checkBox"
                text: qsTr("是否压缩")
            }

            Text {
                id: compression_text
                text: qsTr("压缩算法")
                anchors.verticalCenter: parent.verticalCenter
            }

            ComboBox {
                id: compression_comboBox
                objectName: "compression_comboBox"
                model: ["Huffman", "LZ77"]
            }
        }

        Row {
            id: encryption_row
            y: 150
            spacing: 10

            CheckBox {
                id: encryption_checkBox
                objectName: "encryption_checkBox"
                text: qsTr("是否加密")
            }

            Text {
                id: encryption_text
                text: qsTr("加密算法")
                anchors.verticalCenter: parent.verticalCenter
                font.pointSize: 11
            }

            ComboBox {
                id: encryption_comboBox
                objectName: "encryption_comboBox"
                model: ["AES", "DES"]
            }

            Text {
                id: key_backup_text
                text: qsTr("密码")
                anchors.verticalCenter: parent.verticalCenter
                font.pointSize: 11
            }

            TextField {
                id: key_backup_textField
                objectName: "key_backup_textField"
                width: doBackup_column.width * 0.5
                placeholderText: qsTr("输入密码，默认“123456”")
            }
        }

        Button {
            id: doBackup_button
            objectName: "doBackup_button"
            y: 200
            text: qsTr("备份")
            anchors.horizontalCenter: parent.horizontalCenter

            Connections {
                target: doBackup_button
                onClicked: {
                    mainclass.do_backup()
                }
            }
        }

        ScrollView {
            id: backup_logs_scrollView
            width: parent.width - 20
            height: parent.height * 0.5

            Rectangle {
                color: "white"
                anchors.fill: parent
            }

            ListView {
                id: backup_logs_listView
                x: 0
                y: 0
                width: parent.width
                height: parent.height
                model: backup_logs
                delegate: Text {
                    text: model.display
                    font.pixelSize: Constants.logFont
                }
            }
        }
    }

    Column {
        id: filter_column
        height: parent.height
        width: parent.width * 0.5
        spacing: 10

        Text {
            id: filter_text
            text: qsTr("筛选条件（所有输入勿输引号）")
            anchors.horizontalCenter: parent.horizontalCenter
            font.pointSize: 18
        }

        Row {
            id: fileSize_row
            height: 40

            Text {
                id: fileSize_checkBox
                text: qsTr("  文件大小  ")
                anchors.verticalCenter: parent.verticalCenter
            }

            CheckBox {
                id: from_fileSize_checkBox
                objectName: "from_fileSize_checkBox"
                text: qsTr("最小")
            }

            TextField {
                id: from_fileSize_textField
                objectName: "from_fileSize_textField"
                width: filter_column.width * 0.25
                placeholderText: qsTr("输入字节数")
            }

            CheckBox {
                id: to_fileSize_checkBox
                objectName: "to_fileSize_checkBox"
                text: qsTr("最大")
            }

            TextField {
                id: to_fileSize_textField
                objectName: "to_fileSize_textField"
                width: filter_column.width * 0.25
                placeholderText: qsTr("输入字节数")
            }
        }

        Row {
            id: fileType_row

            CheckBox {
                id: fileType_checkBox
                objectName: "fileType_checkBox"
                text: qsTr("文件类型")
                anchors.verticalCenter: parent.verticalCenter
            }

            Column {
                id: column

                CheckBox {
                    id: normal_fileType_checkBox
                    objectName: "normal_fileType_checkBox"
                    text: qsTr("NORMAL")
                }

                CheckBox {
                    id: hard_link_fileType_checkBox
                    objectName: "hard_link_fileType_checkBox"
                    text: qsTr("HARD_LINK")
                }

                CheckBox {
                    id: symbolic_link_fileType_checkBox
                    objectName: "symbolic_link_fileType_checkBox"
                    text: qsTr("SYMBOLIC_LINK")
                }

                CheckBox {
                    id: directory_fileType_checkBox
                    objectName: "directory_fileType_checkBox"
                    text: qsTr("DIRECTORY")
                }

                CheckBox {
                    id: fifo_fileType_checkBox
                    objectName: "fifo_fileType_checkBox"
                    text: qsTr("FIFO")
                }

                CheckBox {
                    id: other_fileType_checkBox
                    objectName: "other_fileType_checkBox"
                    text: qsTr("OTHER")
                }

                Button {
                    id: all_fileType_checkBox
                    text: qsTr("全选")

                    Connections {
                        target: all_fileType_checkBox
                        onClicked: {
                            normal_fileType_checkBox.checked = true
                            hard_link_fileType_checkBox.checked = true
                            symbolic_link_fileType_checkBox.checked = true
                            directory_fileType_checkBox.checked = true
                            fifo_fileType_checkBox.checked = true
                            other_fileType_checkBox.checked = true
                        }
                    }
                }

                Button {
                    id: none_fileType_checkBox
                    text: qsTr("全不选")

                    Connections {
                        target: none_fileType_checkBox
                        onClicked: {
                            normal_fileType_checkBox.checked = false
                            hard_link_fileType_checkBox.checked = false
                            symbolic_link_fileType_checkBox.checked = false
                            directory_fileType_checkBox.checked = false
                            fifo_fileType_checkBox.checked = false
                            other_fileType_checkBox.checked = false
                        }
                    }
                }
            }
        }

        Row {
            id: pathPatten_row

            CheckBox {
                id: pathPatten_checkBox
                objectName: "pathPatten_checkBox"
                text: qsTr("路径匹配")
            }

            TextField {
                id: pathPatten_textField
                objectName: "pathPatten_textField"
                width: filter_column.width * 0.8
                placeholderText: qsTr("输入正则表达式")
            }
        }

        Row {
            id: namePatten_row

            CheckBox {
                id: namePatten_checkBox
                objectName: "namePatten_checkBox"
                text: qsTr("名称匹配")
            }

            TextField {
                id: namePatten_textField
                objectName: "namePatten_textField"
                width: filter_column.width * 0.8
                placeholderText: qsTr("输入正则表达式")
            }
        }

        Row {
            id: accessTime_row
            x: 0
            y: 440

            Text {
                id: accessTime_checkBox
                text: qsTr("  访问时间  ")
                anchors.verticalCenter: parent.verticalCenter
            }

            Column {
                id: accessTime_column

                Row {
                    id: from_accessTime_row

                    CheckBox {
                        id: from_accessTime_checkBox
                        objectName: "from_accessTime_checkBox"
                        text: qsTr("从")
                    }

                    TextField {
                        id: from_accessTime_textField
                        objectName: "from_accessTime_textField"
                        width: filter_column.width * 0.6
                        placeholderText: qsTr("按照“yyyy-MM-dd HH:mm:ss”格式输入，如“2024-01-01 14:00:00”")
                    }

                    Text {
                        id: second_from_accessTime_text
                        text: qsTr("时刻")
                        anchors.verticalCenter: parent.verticalCenter
                        font.pointSize: 11
                    }
                }

                Row {
                    id: to_accessTime_row
                    CheckBox {
                        id: to_accessTime_checkBox
                        objectName: "to_accessTime_checkBox"
                        text: qsTr("至")
                    }

                    TextField {
                        id: to_accessTime_textField
                        objectName: "to_accessTime_textField"
                        width: filter_column.width * 0.6
                        placeholderText: qsTr("按照“yyyy-MM-dd HH:mm:ss”格式输入，如“2024-01-01 14:00:00”")
                    }

                    Text {
                        id: second_to_accessTime_text
                        text: qsTr("时刻")
                        anchors.verticalCenter: parent.verticalCenter
                        font.pointSize: 11
                    }
                }
            }
        }

        Row {
            id: createTime_row
            x: 0
            y: 440

            Text {
                id: createTime_checkBox
                text: qsTr("  创建时间  ")
                anchors.verticalCenter: parent.verticalCenter
            }

            Column {
                id: createTime_column

                Row {
                    id: from_createTime_row

                    CheckBox {
                        id: from_createTime_checkBox
                        objectName: "from_createTime_checkBox"
                        text: qsTr("从")
                    }

                    TextField {
                        id: from_createTime_textField
                        objectName: "from_createTime_textField"
                        width: filter_column.width * 0.6
                        placeholderText: qsTr("按照“yyyy-MM-dd HH:mm:ss”格式输入，如“2024-01-01 14:00:00”")
                    }

                    Text {
                        id: second_from_createTime_text
                        text: qsTr("时刻")
                        anchors.verticalCenter: parent.verticalCenter
                        font.pointSize: 11
                    }
                }

                Row {
                    id: to_createTime_row
                    CheckBox {
                        id: to_createTime_checkBox
                        objectName: "to_createTime_checkBox"
                        text: qsTr("至")
                    }

                    TextField {
                        id: to_createTime_textField
                        objectName: "to_createTime_textField"
                        width: filter_column.width * 0.6
                        placeholderText: qsTr("按照“yyyy-MM-dd HH:mm:ss”格式输入，如“2024-01-01 14:00:00”")
                    }

                    Text {
                        id: second_to_createTime_text
                        text: qsTr("时刻")
                        anchors.verticalCenter: parent.verticalCenter
                        font.pointSize: 11
                    }
                }
            }
        }

        Row {
            id: modifyTime_row
            x: 0
            y: 520
            Text {
                id: modifyTime_checkBox
                text: qsTr("  修改时间  ")
                anchors.verticalCenter: parent.verticalCenter
            }

            Column {
                id: modifyTime_column
                Row {
                    id: from_modifyTime_row
                    CheckBox {
                        id: from_modifyTime_checkBox
                        objectName: "from_modifyTime_checkBox"
                        text: qsTr("从")
                    }

                    TextField {
                        id: from_modifyTime_textField
                        objectName: "from_modifyTime_textField"
                        width: filter_column.width * 0.6
                        placeholderText: qsTr("按照“yyyy-MM-dd HH:mm:ss”格式输入，如“2024-01-01 14:00:00”")
                    }

                    Text {
                        id: second_from_modifyTime_text
                        text: qsTr("时刻")
                        anchors.verticalCenter: parent.verticalCenter
                        font.pointSize: 11
                    }
                }

                Row {
                    id: to_modifyTime_row
                    CheckBox {
                        id: to_modifyTime_checkBox
                        objectName: "to_modifyTime_checkBox"
                        text: qsTr("至")
                    }

                    TextField {
                        id: to_modifyTime_textField
                        objectName: "to_modifyTime_textField"
                        width: filter_column.width * 0.6
                        placeholderText: qsTr("按照“yyyy-MM-dd HH:mm:ss”格式输入，如“2024-01-01 14:00:00”")
                    }

                    Text {
                        id: second_to_modifyTime_text
                        text: qsTr("时刻")
                        anchors.verticalCenter: parent.verticalCenter
                        font.pointSize: 11
                    }
                }
            }
        }

        // Row {
        //     id: excludePath_row

        //     CheckBox {
        //         id: excludePath_checkBox
        //         text: qsTr("排除路径")
        //     }

        //     TextField {
        //         id: excludePath_textField
        //         width: 640
        //         placeholderText: qsTr("多个路径以英文分号（;）分隔，末项无需分号，勿输多余空格。如“bin/run;docs/help”")
        //     }
        // }

        // Row {
        //     id: users_row
        //     CheckBox {
        //         id: users_checkBox
        //         text: qsTr("用户筛选")
        //     }

        //     ComboBox {
        //         id: users_comboBox
        //         width: 140
        //         model: ["只选用户", "排除用户"]
        //     }

        //     TextField {
        //         id: users_textField
        //         width: 500
        //         placeholderText: qsTr("多个用户以英文分号（;）分隔，末项无需分号，勿输多余空格。")
        //     }
        // }

        // Row {
        //     id: groups_row
        //     CheckBox {
        //         id: groups_checkBox
        //         text: qsTr("户组筛选")
        //     }

        //     ComboBox {
        //         id: groups_comboBox
        //         width: 140
        //         model: ["只选用户组", "排除用户组"]
        //     }

        //     TextField {
        //         id: groups_textField
        //         width: 500
        //         placeholderText: qsTr("多个用户以英文分号（;）分隔，末项无需分号，勿输多余空格。")
        //     }
        // }
        Row {
            id: all_none_filter_row
            spacing: 10
            anchors.horizontalCenter: parent.horizontalCenter

            Button {
                id: all_filter_checkBox
                text: qsTr("全选")

                Connections {
                    target: all_filter_checkBox
                    onClicked: {
                        fileSize_checkBox.checked = true
                        from_fileSize_checkBox.checked = true
                        to_fileSize_checkBox.checked = true
                        fileType_checkBox.checked = true
                        pathPatten_checkBox.checked = true
                        namePatten_checkBox.checked = true
                        accessTime_checkBox.checked = true
                        from_accessTime_checkBox.checked = true
                        to_accessTime_checkBox.checked = true
                        createTime_checkBox.checked = true
                        from_createTime_checkBox.checked = true
                        to_createTime_checkBox.checked = true
                        modifyTime_checkBox.checked = true
                        from_modifyTime_checkBox.checked = true
                        to_modifyTime_checkBox.checked = true
                    }
                }
            }

            Button {
                id: none_filter_checkBox
                text: qsTr("全不选")

                Connections {
                    target: none_filter_checkBox
                    onClicked: {
                        fileSize_checkBox.checked = false
                        from_fileSize_checkBox.checked = false
                        to_fileSize_checkBox.checked = false
                        fileType_checkBox.checked = false
                        pathPatten_checkBox.checked = false
                        namePatten_checkBox.checked = false
                        accessTime_checkBox.checked = false
                        from_accessTime_checkBox.checked = false
                        to_accessTime_checkBox.checked = false
                        createTime_checkBox.checked = false
                        from_createTime_checkBox.checked = false
                        to_createTime_checkBox.checked = false
                        modifyTime_checkBox.checked = false
                        from_modifyTime_checkBox.checked = false
                        to_modifyTime_checkBox.checked = false
                        // excludePath_checkBox.checked = false
                        // users_checkBox.checked = false
                        // groups_checkBox.checked = false
                    }
                }
            }
        }
    }

    states: [
        State {
            name: "clicked"
        }
    ]
}
