// This program changes the text of listview items in the System Configuration dialog (msconfig.exe) based on entries in a text file.
/* The text file should be in the following format, quotation marks included:
"Stopped" "Bogus Status"
"Microsoft Corporation" "Bogus Company"
*/
// Currently only finds & replaces full strings, no substrings
// Note: If msconfig is a 64 bit process, this program also needs to be compiled as x64

// Code based on https://stackoverflow.com/a/12683120

#include "stdafx.h"
#include "filereader.h"

#include <windows.h>
#include <CommCtrl.h>
#include <iostream>


using namespace std;

#define LSTR	260

// ** EnumChildProc - This function is called for each child hwnd of the System Configuration dialog
BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lParam)
{
	// Ignore hidden controls
	if (IsWindowVisible(hwnd) == FALSE) return TRUE;

	// Check the current hwnd type
	TCHAR szClassName[LSTR];
	if (GetClassName(hwnd, szClassName, LSTR) > 0) {

		// If hwnd is a List View control...
		if (_tcscmp(szClassName, WC_LISTVIEW) == 0) {

			// ListView messages that pass around buffers only work within the address space of the process that owns the ListView
			// So allocate some memory within the msconfig process
			DWORD dwProcessId;
			GetWindowThreadProcessId(hwnd, &dwProcessId);
			HANDLE hProcess = OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, dwProcessId);

			LVITEM* pLvItem = (LVITEM*)VirtualAllocEx(hProcess, NULL, sizeof(LVITEM), MEM_COMMIT, PAGE_READWRITE);
			LPTSTR pText = (LPTSTR)VirtualAllocEx(hProcess, NULL, sizeof(TCHAR) * LSTR, MEM_COMMIT, PAGE_READWRITE);

			// How many columns are there?
			HWND hwndHeader = ListView_GetHeader(hwnd);
			int iColumnCount = Header_GetItemCount(hwndHeader);

			// Iterate over all of the listview items
			int iCount = ListView_GetItemCount(hwnd);
			for (int iCurItem = 0; iCurItem < iCount; iCurItem++) {

				for (int iCurColumn = 0; iCurColumn < iColumnCount; iCurColumn++) {

					LVITEM lvItem = { 0 };
					lvItem.mask = LVIF_TEXT;
					lvItem.iItem = iCurItem;
					lvItem.iSubItem = iCurColumn;
					lvItem.pszText = pText; // Pointer to the buffer that receives the item text
					lvItem.cchTextMax = LSTR;

					// Write our local lvItem to the memory we allocated in msconfig
					WriteProcessMemory(hProcess, pLvItem, &lvItem, sizeof(LVITEM), NULL);

					// Check the text of the current item
					int iCharsRead = int(SendMessage(hwnd, LVM_GETITEMTEXT, iCurItem, LPARAM(pLvItem)));
					if (iCharsRead > 0) {

						// Copy the remote buffer containing the result into szText
						TCHAR szText[LSTR] = { 0 };
						ReadProcessMemory(hProcess, pText, &szText[0], sizeof(TCHAR) * iCharsRead, NULL);

						// Does the Find/Replace map contain the string found in the listview item?
						map<string, string>* pFindReplaceData = (map<string, string>*)(lParam);
						if (pFindReplaceData == nullptr) break;

						map<string, string>::iterator itMap = pFindReplaceData->find(szText);
						if (itMap != pFindReplaceData->end()) {
							// We can reuse the lvItem struct for LVM_SETITEMTEXT, just change its text first
							WriteProcessMemory(hProcess, pText, itMap->second.c_str(), LSTR, NULL);
							SendMessage(hwnd, LVM_SETITEMTEXT, iCurItem, LPARAM(pLvItem));
						}
					}

				}

			}

			VirtualFreeEx(hProcess, pText, 0, MEM_RELEASE);
			VirtualFreeEx(hProcess, pLvItem, 0, MEM_RELEASE);
			CloseHandle(hProcess);
		}

	}

	return TRUE;
}

// ** IsAppRunningAsAdmin - Returns true if this instance of the app has admin privileges
BOOL IsAppRunningAsAdmin()
{
	// https://www.codeproject.com/Articles/320748/Haephrati-Elevating-during-runtime
	BOOL bIsRunAsAdmin = FALSE;
	PSID pAdminGroup = NULL; // SID of the admin group

	// Allocate and initialize pAdministratorsGroup
	SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
	BOOL bInitSuccess = AllocateAndInitializeSid(
		&NtAuthority,
		2,
		SECURITY_BUILTIN_DOMAIN_RID,
		DOMAIN_ALIAS_RID_ADMINS,
		0, 0, 0, 0, 0, 0,
		&pAdminGroup
	);

	// Determine if the SID of the admin group is enabled in the primary access token of the process
	if (bInitSuccess) CheckTokenMembership(NULL, pAdminGroup, &bIsRunAsAdmin);

	// Cleanup
	if (pAdminGroup != NULL) FreeSid(pAdminGroup);

	return bIsRunAsAdmin;
}

// ** RunAsAdmin - Launches this app with admin privileges
void RunAsAdmin()
{
	// https://www.codeproject.com/Articles/320748/Haephrati-Elevating-during-runtime
	TCHAR szAppName[LSTR];
	if (GetModuleFileName(NULL, szAppName, LSTR) != 0) {
		// Launch self as admin
		SHELLEXECUTEINFO shellExeInfo = { sizeof(shellExeInfo) };
		shellExeInfo.lpVerb = TEXT("runas");
		shellExeInfo.lpFile = szAppName;
		shellExeInfo.hwnd = NULL;
		shellExeInfo.nShow = SW_NORMAL;

		ShellExecuteEx(&shellExeInfo);
	}
}

int main()
{
	// Make sure we're running as admin...(otherwise we can't mess with the system config window)
	if (!IsAppRunningAsAdmin()) {
		RunAsAdmin();
		return 0; // Close this instance and let the admin instance execute
	}

	// Get rid of the console window
	FreeConsole();

	// Open the "Find & Replace" text file
	TCHAR szFilePath[LSTR];
	BOOL bFileOpened = FALSE;
	while (!bFileOpened) {
		szFilePath[0] = 0;
		OPENFILENAME ofn;
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = NULL;
		ofn.lpstrFile = szFilePath;
		ofn.nMaxFile = sizeof(szFilePath);
		ofn.lpstrFilter = TEXT("Text\0 * .TXT\0");
		ofn.nFilterIndex = 1;
		ofn.lpstrTitle = TEXT("Open Find & Replace File");
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
		bFileOpened = GetOpenFileName(&ofn);
		
		// If the user failed to open a text file, show a dialog to let them retry or close the program.
		if (!bFileOpened) {
			if (MessageBox(NULL, TEXT("Select a text file that contains find & replace text. E.g.: \n \"Stopped\" \"Running\""), TEXT("Please select a text file"), MB_RETRYCANCEL | MB_ICONWARNING) != IDRETRY) return 0;
		}
	}


	FileReader fileReader(szFilePath);
	map<string, string> vFindReplaceData; // Key is the string to find, Value is the string to replace it with

	// Continue to check if a "System Configuration" window is open
	while (true) {
		HWND hwndSC = FindWindow(0, TEXT("System Configuration"));
		if (hwndSC != NULL) {

			// Update our find & replace data
			if (fileReader.HasFileChanged()) {
				vFindReplaceData.clear();
				fileReader.ReadFile(vFindReplaceData);
			}

			// Iterate through each child window in the system config window
			EnumChildWindows(hwndSC, EnumChildProc, LPARAM(&vFindReplaceData));
		}
	}

	return 0;
}
