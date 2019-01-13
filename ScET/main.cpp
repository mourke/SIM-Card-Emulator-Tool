#include "ScET.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[]) {
	QApplication a(argc, argv);
	ScET w;
	w.show();
	return a.exec();
}
