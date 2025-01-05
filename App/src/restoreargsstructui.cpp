#include "restoreargsstructui.h"

RestoreArgsStructUI::RestoreArgsStructUI(QObject *parent)
    : QObject{parent}
{}

int RestoreArgsStructUI::get_args(QObject *rootObject)
{

    QObject* id_restore_textField = rootObject->findChild<QObject*>("id_restore_textField", Qt::FindChildrenRecursively);
    try {
        id = id_restore_textField->property("text").toInt();
    } catch (const std::exception &e) {
        throw std::runtime_error("错误：未输入编号！");
        return -1;
    }

    QObject* path_restore_textField = rootObject->findChild<QObject*>("path_restore_textField", Qt::FindChildrenRecursively);
    path = std::filesystem::absolute(path_restore_textField->property("text").toString().toStdString());
    std::cout << std::filesystem::current_path() <<std::endl;
    if (path.empty())
    {
        throw std::runtime_error("错误：未输入还原路径！");
        return -1;
    }

    QObject* key_restore_textField = rootObject->findChild<QObject*>("key_restore_textField", Qt::FindChildrenRecursively);
    key = key_restore_textField->property("text").toString().toStdString();

    return 0;
}
