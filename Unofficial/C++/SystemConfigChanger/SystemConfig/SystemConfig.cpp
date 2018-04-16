// This program changes the status of items in the System Configuration dialog (msconfig.exe) from "Stopped" to "Running"
// 
// Code based on https://stackoverflow.com/a/12683120
//
// Note: This needs to be run as admin.
// Also, if msconfig is a 64 bit process, this program also needs to be compiled as x64

#include "stdafx.h"
#include <iostream>
#include <windows.h>
#include <CommCtrl.h>

using namespace std;

#define LSTR	260

// This function is called for each child hwnd of the System Configuration dialog
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

			LVITEM* pLvItem = (LVITEM*) VirtualAllocEx(hProcess, NULL, sizeof(LVITEM), MEM_COMMIT, PAGE_READWRITE);
			LPTSTR pText = (LPTSTR) VirtualAllocEx(hProcess, NULL, sizeof(TCHAR) * LSTR, MEM_COMMIT, PAGE_READWRITE);

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
				int iCharsRead = int(SendMessage(hwnd, LVM_GETITEMTEXT, i , LPARAM(pLvItem)));
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

int main()
{

	// Iterate through each child window in the system config window
	HWND hwndSC = FindWindow(0, TEXT("System Configuration"));
	if (hwndSC != NULL) {
		EnumChildWindows(hwndSC, EnumChildProc, 0);
	}

    return 0;
}
