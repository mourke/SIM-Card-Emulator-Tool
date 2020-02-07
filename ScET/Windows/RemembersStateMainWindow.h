#ifndef REMEMBERSSTATEMAINWINDOW_H
#define REMEMBERSSTATEMAINWINDOW_H

#include <QMainWindow>

class RemembersStateMainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit RemembersStateMainWindow(QWidget *parent = nullptr);

protected:
    void closeEvent(QCloseEvent *closeEvent) override;
    void restoreState();
    void saveState();
};

#endif // REMEMBERSSTATEMAINWINDOW_H
