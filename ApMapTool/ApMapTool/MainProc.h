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
	void SaveMapFile(std::string str) const;

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

	int getGoldValue() const {
		return m_goldValue;
	}

	int getSilverValue() const {
		return m_silverValue;
	}

	void SetGoldValue(int value) {
		m_goldValue = value;
	}

	void SetSilverValue(int value) {
		m_silverValue = value;
	}

	void AutoSaveFile();
	void AutoSaveFileWithPath();

	void DebugGame();

public:
	static void Split(std::vector<std::string>& lst, std::string input, const std::string& separators, bool remove_empty);
	static std::string replace_all(const std::string& message, const std::string& pattern, const std::string& replace);

private:
	std::string m_filePath;
	std::string m_fileName;

	std::string m_exePath;

	int m_goldValue = 0;
	int m_silverValue = 0;
};

