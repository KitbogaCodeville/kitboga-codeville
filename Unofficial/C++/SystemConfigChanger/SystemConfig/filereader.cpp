#include "stdafx.h"

#include "filereader.h"

#include <fstream>
#include <sstream>


FileReader::FileReader(string _sFilePath)
: sFilePath(_sFilePath)
{
	wstring ws;
	ws.assign(sFilePath.begin(), sFilePath.end());
	hFile = CreateFile(ws.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	timeLastRead.dwHighDateTime = 0;
	timeLastRead.dwLowDateTime = 0;
}

void FileReader::ReadFile(map<wstring, wstring>& vFindReplaceData)
{
	std::wifstream inFile(sFilePath);

	// Read each line of the file
	wstring line;
	while (std::getline(inFile, line)) {
		if (line.rfind(wstring(L"//"), 0) == 0) continue; // Ignore lines that start with "//"

		// Find string A
		size_t uStartA = line.find('"');
		if (uStartA == std::string::npos) continue;
		uStartA++; // Don't include the opening "
		size_t uEndA = line.find('"', uStartA);
		if (uEndA == std::string::npos) continue;
		wstring stringA = line.substr(uStartA, uEndA - uStartA);

		// Find string B
		size_t uStartB = line.find('"', uEndA + 1);
		if (uStartB == std::string::npos) continue;
		uStartB++; // Don't include the opening "
		size_t uEndB = line.find('"', uStartB);
		if (uEndB == std::string::npos) continue;
		wstring stringB = line.substr(uStartB, uEndB - uStartB);

		// Add the strings to vFindReplaceData
		vFindReplaceData.insert(pair<wstring, wstring>(stringA, stringB));

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
