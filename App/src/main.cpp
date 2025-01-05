// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQmlProperty>

#include "App/autogen/environment.h"

#include "mainclass.h"

int main(int argc, char **argv)
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
    MainClass* mainclass = new MainClass(&engine);
    context->setContextProperty("mainclass", mainclass);

    TopTableModel* table = new TopTableModel(&engine);
    context->setContextProperty("toptablemodel", table);
    mainclass->set_table(table);

    LogsModel* backup_logs = new LogsModel(&engine);
    context->setContextProperty("backup_logs", &backup_logs->logs);
    mainclass->set_backup_logs(backup_logs);

    LogsModel* restore_logs = new LogsModel(&engine);
    context->setContextProperty("restore_logs", &restore_logs->logs);
    mainclass->set_restore_logs(restore_logs);

    context->setContextProperty("db_path", Database::get_db_path());

    engine.load(url);

    if (engine.rootObjects().isEmpty())
        return -1;

    mainclass->show_table();

    return app.exec();
}
