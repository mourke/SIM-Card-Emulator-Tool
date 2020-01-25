#include "FileManager.h"
#include <QString>
#include <QFile>
#include <QTextStream>
#include <exception>

FileManager::FileManager() {

}

void FileManager::saveFile(const QString &fileName, const QString &contents) {
	QFile file(fileName);
	if (file.open(QIODevice::WriteOnly)) {
		QTextStream out(&file);
		out << contents;
		file.close();
	} else {
		throw std::invalid_argument(file.errorString().toStdString());
	}
}

std::vector<uint8_t> FileManager::openFile(const QString &fileName) {
	QFile file(fileName);

	if (file.open(QIODevice::ReadOnly)) {
		QTextStream in(&file);
		std::vector<uint8_t> data;
		while (!in.atEnd()) {
			QString input = in.readLine();
			int inputLength = input.length();
			if (inputLength == 0) continue;
			for (int i = 0; i + 1 < inputLength; i += 2) {
				uint8_t hex = 0;
				QChar first = input[i].toLower();
				QChar second = input[i + 1].toLower();
				if (first.isDigit()) {
					hex = first.digitValue() * 16;
				} else {
					hex += (first.toLatin1() - 'a' + 10) * 16;
				}

				if (second.isDigit()) {
					hex += second.digitValue();
				} else {
					hex += second.toLatin1() - 'a' + 10;
				}
				data.push_back(hex);
			}
		}

		file.close();
		return data;
	} else {
		throw std::invalid_argument(file.errorString().toStdString());
	}
}

