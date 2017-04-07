#pragma once
#include <string>
#include <vector>

class MainProc {
public:
	MainProc();
	~MainProc();

	void setFilePath(std::string path);
	void ReadMapFile();
	void SaveMapFile() const;

	std::string getTitleName() const {
		std::string title = "±«µµ ¾ÓÆÎ ¸ÊÅø - ";
		title.append(m_fileName);
		return title;
	}

	std::string getFileName() const {
		return m_fileName;
	}

	std::string getFilePath() const {
		return m_filePath;
	}

	void AutoSaveFile();
	void AutoSaveFileWithPath();

public:
	static void Split(std::vector<std::string>& lst, std::string input, const std::string& separators, bool remove_empty);
	static std::string replace_all(const std::string& message, const std::string& pattern, const std::string& replace);

private:
	std::string m_filePath;
	std::string m_fileName;
};

