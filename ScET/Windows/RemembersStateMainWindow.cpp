#include "RemembersStateMainWindow.h"
#include <QSettings>

RemembersStateMainWindow::RemembersStateMainWindow(QWidget *parent) : QMainWindow(parent) {

}

void RemembersStateMainWindow::restoreState() {
    QSettings settings;

    settings.beginGroup("RemembersStateMainWindow");

    restoreGeometry(settings.value("geometry", saveGeometry()).toByteArray());
    move(settings.value("pos", pos()).toPoint());
    resize(settings.value("size", size()).toSize());

    if (settings.value("maximized", isMaximized()).toBool()) {
        showMaximized();
    }

    settings.endGroup();
}

void RemembersStateMainWindow::saveState() {
    QSettings settings;

    settings.beginGroup("RemembersStateMainWindow");

    settings.setValue("geometry", saveGeometry());
    settings.setValue("maximized", isMaximized());

    if (!isMaximized()) {
        settings.setValue("pos", pos());
        settings.setValue("size", size());
    }

    settings.endGroup();
}

void RemembersStateMainWindow::closeEvent(QCloseEvent *closeEvent) {
    QMainWindow::closeEvent(closeEvent);
    saveState();
}
