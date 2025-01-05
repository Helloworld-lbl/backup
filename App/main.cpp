// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQmlProperty>

#include "autogen/environment.h"
// #include "backupargsstructui.h"
// #include "toptablemodel.h"

int main(int argc, char *argv[])
{
    set_qt_environment();
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(mainQmlFile);
    QObject::connect(
                &engine, &QQmlApplicationEngine::objectCreated, &app,
                [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    engine.addImportPath(QCoreApplication::applicationDirPath() + "/qml");
    engine.addImportPath(":/");

    QQmlContext* context = engine.rootContext();
    // context->setContextProperty("backupargsstructui", new BackupArgsStructUI(&engine));

    // qmlRegisterType<BackupArgsStructUI>("com.mycompany", 1, 0, "BackupArgsStructUI");
    // qmlRegisterType<TopTableModel>("TopTableModel",1,0,"TopTableModel");

    engine.load(url);

    // QObject *rootObject = engine.rootObjects().first();
    // QObject *myItem = rootObject->findChild<QObject*>("button");

    // // QObject *myItem = engine.findChild<QObject*>("encryption_comboBox");
    // if (!myItem) {
    //     return -1;
    // }

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
