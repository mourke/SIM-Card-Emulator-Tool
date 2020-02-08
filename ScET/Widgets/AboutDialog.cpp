#include "AboutDialog.h"
#include <QApplication>

AboutDialog::AboutDialog(QWidget *parent) : QDialog(parent) {
    ui.setupUi(this);

    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
	ui.appVersion->setText(ui.appVersion->text() + " " + QApplication::applicationVersion());

    auto font = this->font();
    font.setFamily("MS Shell Dlg 2");
    setFont(font);
}
