// This program changes the status of items in the System Configuration dialog (msconfig.exe) from "Stopped" to "Running"
// 
// Code based on https://stackoverflow.com/a/12683120
//
// Note: If msconfig is a 64 bit process, this program also needs to be compiled as x64

#include "stdafx.h"
#include <iostream>
#include <windows.h>
#include <CommCtrl.h>

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

			// Iterate over all of the listview items
			int iCount = ListView_GetItemCount(hwnd);
			for (int i = 0; i < iCount; i++) {

				LVITEM lvItem = { 0 };
				lvItem.mask = LVIF_TEXT;
				lvItem.iItem = i;
				lvItem.iSubItem = 2;	// Status column
				lvItem.pszText = pText; // Pointer to the buffer that receives the item text
				lvItem.cchTextMax = LSTR;

				// Write our local lvItem to the memory we allocated in msconfig
				WriteProcessMemory(hProcess, pLvItem, &lvItem, sizeof(LVITEM), NULL);

				// Check the text of the current item
				int iCharsRead = int(SendMessage(hwnd, LVM_GETITEMTEXT, i, LPARAM(pLvItem)));
				if (iCharsRead > 0) {

					// Copy the remote buffer containing the result into szText
					TCHAR szText[LSTR] = { 0 };
					ReadProcessMemory(hProcess, pText, &szText[0], sizeof(TCHAR) * iCharsRead, NULL);

					// If the item text is "Stopped" change it to something else
					if (_tcscmp(szText, TEXT("Stopped")) == 0) {
						// We can reuse the lvItem struct for LVM_SETITEMTEXT, just change its text first
						WriteProcessMemory(hProcess, pText, TEXT("Running"), LSTR, NULL);
						SendMessage(hwnd, LVM_SETITEMTEXT, i, LPARAM(pLvItem));
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

// ** IsAppRunningAsAdmin - returns true if this instance of the app has admin privileges
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
	wchar_t szAppName[LSTR];
	if (GetModuleFileName(NULL, szAppName, LSTR) != 0) {
		// Launch self as admin
		SHELLEXECUTEINFO shellExeInfo = { sizeof(shellExeInfo) };
		shellExeInfo.lpVerb = L"runas";
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

	// Continue to check if a "System Configuration" window is open
	while (true) {
		HWND hwndSC = FindWindow(0, TEXT("System Configuration"));
		if (hwndSC != NULL) {
			// Iterate through each child window in the system config window
			EnumChildWindows(hwndSC, EnumChildProc, 0);
		}
	}

	return 0;
}
