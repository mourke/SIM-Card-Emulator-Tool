#ifndef OPENFILEAPPLICATION_H
#define OPENFILEAPPLICATION_H

#include <QtCore/QtGlobal>

#ifndef QAPPLICATION_CLASS
  #define QAPPLICATION_CLASS QCoreApplication
#endif

#include QT_STRINGIFY(QAPPLICATION_CLASS)

class OpenFileApplication : public QAPPLICATION_CLASS {
    Q_OBJECT

    typedef QAPPLICATION_CLASS app_t;
public:
    OpenFileApplication(int &argc, char **argv);

    bool event(QEvent *event) override;

signals:
    void openFile(const QString &fileName);
};

#endif // OPENFILEAPPLICATION_H
