// EventViewerIcons.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include "Shlwapi.h"
#include <windows.h>
#include <Psapi.h>
#include <tchar.h>
#include <CommCtrl.h>
#include <iostream>
#include <set>

#include "..\EventViewerIconsDLL\dllmain.h"

using namespace std;

#define LSTR	260
wchar_t InjLibName[] = L"EventViewerIcons.dll";
char InjectMethodName[] = "InjectAndStartThread";
char EjectMethodName[] = "EjectLibrary";

typedef HANDLE(*pInjectMethod)(DWORD, LPVOID);
typedef void(*pEjectMethod)(DWORD);


// ** EnumWindowsProc	- Iterates over every open window
//						* lParam is a pointer to a set<HWND>. All open windows with the title of "Event Viewer" are added to this set
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	set<HWND>* psWindows = (set<HWND>*)(lParam);
	if (psWindows == nullptr) return FALSE;

	// Get the name of the window
	TCHAR szWindowName[LSTR];
	if (GetWindowText(hwnd, szWindowName, LSTR) > 0) {

		// If the window name is "Event Viewer" add it to our vecotr of HWNDs to process
		if (_tcscmp(szWindowName, TEXT("Event Viewer")) == 0) {
			psWindows->insert(hwnd);
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
	// Make sure we're running as admin...(otherwise we won't have permission to write memory to another process)
	if (!IsAppRunningAsAdmin()) {
		RunAsAdmin();
		return 0; // Close this instance and let the admin instance execute
	}

	// Get rid of the console window
	FreeConsole();

	// Get the full path for this executable - the DLL and icons should be in the same folder
	TCHAR szPathExe[LSTR];
	GetModuleFileName(NULL, szPathExe, LSTR);
	wstring sDirectory(szPathExe);
	sDirectory = sDirectory.substr(0, sDirectory.find_last_of(TEXT("/\\")) + 1); // +1 to include the last "/\\"

	// Full paths for the DLL and icons
	wstring sPathDLL = sDirectory + TEXT("EventViewerIconsDLL.dll");
	wstring sPathIcon = sDirectory + TEXT("check.bmp");

	// Make sure the paths are valid
	do {
		wstring sError;
		if (!PathFileExists(sPathDLL.c_str())) sError += TEXT("Missing ") + sPathDLL + TEXT("\n\n");
		if (!PathFileExists(sPathIcon.c_str())) sError += TEXT("Missing ") + sPathIcon + TEXT("\n\n");

		if (sError.empty()) break; // Everything is valid - we can continue with the execution of the application
		else if (MessageBox(NULL, sError.c_str(), NULL, MB_RETRYCANCEL | MB_ICONERROR) != IDRETRY) return 0; // Exit the application if the user has errors and doesn't retry

	} while (true);

	// Continuously search for an "Event Viewer" window
	set<HWND> sWindowsProcessed;
	while (true) {

		// Get a set of all open "Event Viewer" windows
		set<HWND> sWindowsToProcess;
		EnumWindows(EnumWindowsProc, LPARAM(&sWindowsToProcess));

		for (set<HWND>::iterator iterWindow = sWindowsToProcess.begin(); iterWindow != sWindowsToProcess.end(); ++iterWindow) {
			if (sWindowsProcessed.find(*iterWindow) == sWindowsProcessed.end()) { // Only process windows that we haven't already modified

				// Inject our DLL into the current window
				DWORD dwProcessID;
				GetWindowThreadProcessId(*iterWindow, &dwProcessID);
				HMODULE injLib = LoadLibrary(sPathDLL.c_str());


				pInjectMethod injectMethod = (pInjectMethod)GetProcAddress(injLib, InjectMethodName);
				pEjectMethod ejectMethod = (pEjectMethod)GetProcAddress(injLib, EjectMethodName);

				SharedData sharedData;
				sharedData.hwndWindow = *iterWindow;
				wcsncpy_s(sharedData.szPathIcon, sPathIcon.c_str(), LSTR);

				HANDLE waitHandle = injectMethod(dwProcessID, LPVOID(&sharedData));

				if (waitHandle == NULL) {
					break;
				}

				DWORD dwInjected = WaitForSingleObject(waitHandle, 9001);
				ejectMethod(dwProcessID);

				// Mark this window as processed so we don't keep changing it
				sWindowsProcessed.insert(*iterWindow);
			}
		}
	}

    return 0;
}
