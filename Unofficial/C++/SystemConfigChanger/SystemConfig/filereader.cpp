#include "stdafx.h"

#include "filereader.h"

#include <fstream>
#include <sstream>

FileReader::FileReader(string _sFilePath)
: sFilePath(_sFilePath)
{
	hFile = CreateFile(sFilePath.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	timeLastRead.dwHighDateTime = 0;
	timeLastRead.dwLowDateTime = 0;
}

void FileReader::ReadFile(map<string, string>& vFindReplaceData)
{
	ifstream inFile(sFilePath);

	// Read each line of the file
	string line;
	while (getline(inFile, line)) {
		if (line.rfind(string(TEXT("//")), 0) == 0) continue; // Ignore lines that start with "//"

		// Find string A
		size_t uStartA = line.find('"');
		if (uStartA == string::npos) continue;
		uStartA++; // Don't include the opening "
		size_t uEndA = line.find('"', uStartA);
		if (uEndA == string::npos) continue;
		string stringA = line.substr(uStartA, uEndA - uStartA);

		// Find string B
		size_t uStartB = line.find('"', uEndA + 1);
		if (uStartB == string::npos) continue;
		uStartB++; // Don't include the opening "
		size_t uEndB = line.find('"', uStartB);
		if (uEndB == string::npos) continue;
		string stringB = line.substr(uStartB, uEndB - uStartB);

		// Add the strings to vFindReplaceData
		vFindReplaceData.insert(pair<string, string>(stringA, stringB));

	}

	// Update the last time we read the file
	GetFileTime(hFile, NULL, NULL, &timeLastRead);
}


bool FileReader::HasFileChanged() const
{
	FILETIME timeLastWritten;
	if (GetFileTime(hFile, NULL, NULL, &timeLastWritten)) {
		return ((timeLastWritten.dwHighDateTime != timeLastRead.dwHighDateTime) || (timeLastWritten.dwLowDateTime != timeLastRead.dwLowDateTime));
	}

	return false;
}
