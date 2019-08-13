#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <vector>
#include <cstdint>

class QString;

class FileManager {

public:

	/** The shared manager instance */
	static FileManager & sharedManager() {
		static FileManager instance;
		return instance;
	}

	// prevent accidental copies
	FileManager(FileManager const&) = delete;
	void operator=(FileManager const&) = delete;

	void saveFile(const QString &fileName, const QString &contents);
	std::vector<uint8_t> openFile(const QString &fileName);
private:
	FileManager();
};

#endif // FILE_MANAGER_H
