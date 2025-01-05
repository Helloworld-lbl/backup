// Copyright (C) 2021 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

import QtQuick
import QtQuick.Controls
import Backup

ApplicationWindow {
    id: window
    width: Constants.width
    height: Constants.height

    visible: true
    title: "Backup"

    Rectangle {
        id: mainScreen
        width: parent.width
        height: parent.height

        color: Constants.backgroundColor

        Button {
            id: backup_rectangle_button
            x: 0
            y: 0
            text: qsTr("备份")

            Connections {
                target: backup_rectangle_button
                onClicked: {
                    backup.visible = true
                    restore.visible = false
                    config.visible = false
                }
            }
        }

        Button {
            id: restore_rectangle_button
            x: 106
            y: 0
            text: qsTr("还原")

            Connections {
                target: restore_rectangle_button
                onClicked: {
                    mainclass.show_table()
                    backup.visible = false
                    restore.visible = true
                    config.visible = false
                }
            }
        }

        Button {
            id: config_rectangle_button
            x: 212
            y: 0
            text: qsTr("设置")

            Connections {
                target: config_rectangle_button
                onClicked: {
                    backup.visible = false
                    restore.visible = false
                    config.visible = true
                }
            }
        }
    }

    Backup {
        id: backup
        y: 40
    }

    Restore {
        id: restore
        y: 40
        visible: false
    }

    Config {
        id: config
        y: 40
        visible: false
    }
}

