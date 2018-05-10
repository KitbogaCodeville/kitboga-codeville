#pragma once
#include <map>
#include <string>
#include <windows.h>

using namespace std;

class FileReader {
public:
	explicit FileReader(string sFilePath);

	void ReadFile(map<string, string>& vFindReplaceData); // Populates vFindReplaceData with strings from the file
	bool HasFileChanged() const; // Has the file been saved since the last ReadFile call?
	
private:
	string sFilePath;
	HANDLE hFile;
	FILETIME timeLastRead;
};
