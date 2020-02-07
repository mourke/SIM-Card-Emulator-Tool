#include "OpenFileApplication.h"
#include <QFileOpenEvent>
#include <QDebug>

OpenFileApplication::OpenFileApplication(int &argc, char **argv) : app_t(argc, argv) {
}

bool OpenFileApplication::event(QEvent *event) {
    if (event->type() == QEvent::FileOpen) {
        QFileOpenEvent *openEvent = static_cast<QFileOpenEvent *>(event);
        emit openFile(openEvent->file());
    }

    return app_t::event(event);
}
