#ifndef ABOUT_DIALOG_H
#define ABOUT_DIALOG_H

#include <QDialog>
#include "ui_AboutDialog.h"

class AboutDialog : public QDialog {
	Q_OBJECT

public:
    AboutDialog(QWidget *parent = Q_NULLPTR);

private:
    Ui::AboutDialog ui;
};

#endif
