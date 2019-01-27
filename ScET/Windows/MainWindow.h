#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"

class MainWindow: public QMainWindow {
	Q_OBJECT

public:
	MainWindow(QWidget *parent = Q_NULLPTR);

private:
	Ui::MainWindow ui;
};

#endif // MAINWINDOW_H