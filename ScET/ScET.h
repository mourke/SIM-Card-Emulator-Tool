#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_ScET.h"

class ScET : public QMainWindow
{
	Q_OBJECT

public:
	ScET(QWidget *parent = Q_NULLPTR);

private:
	Ui::ScETClass ui;
};
