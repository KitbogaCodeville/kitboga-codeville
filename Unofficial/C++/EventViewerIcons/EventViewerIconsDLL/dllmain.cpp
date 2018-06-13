// DLL loading and unloading based on: https://www.codeproject.com/Articles/90271/Remote-threads-basics-Part

#include "stdafx.h"
#include "dllmain.h"
#include <Psapi.h>
#include <windows.h>

#define LSTR	260
using namespace std;

// DLL Loading address
HMODULE hThisModule = NULL;

// Exported name of the method which will be called remotely
char StartingMethod[] = "RemoteMethod"; // Functions aren't decorated in x64

// ** InjectAndStartThread	- Injects DLL into target process and starts a remote thread.
//							- Returns handle to the thread that was started
extern "C" __declspec(dllexport) HANDLE InjectAndStartThread(DWORD dwTargetProcessID, LPVOID lpParam)
{

	FARPROC localMethodAddr = GetProcAddress(hThisModule, StartingMethod);
	if (localMethodAddr == NULL) return FALSE;
	INT_PTR methodDelta = (INT_PTR)localMethodAddr - (INT_PTR)hThisModule;

	HANDLE hTargetProcess = NULL;
	wchar_t szModuleName[MAX_PATH];
	LPVOID pRemotePathBuffer = NULL;
	LPVOID pRemoteSharedData = NULL;
	HANDLE hRemoteThread = NULL;
	HANDLE result = NULL;

	__try
	{
		hTargetProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwTargetProcessID);
		if (hTargetProcess == NULL) __leave;

		// Get the name of this .dll and store that string in the remote process
		memset(szModuleName, 0, sizeof(wchar_t) * LSTR);
		GetModuleFileNameW(hThisModule, szModuleName, LSTR);
		szModuleName[LSTR - 1] = 0;

		// Allocate the string buffer in the remote process
		int iRemoteMemSize = sizeof(wchar_t) * (lstrlenW(szModuleName) + 1);
		pRemotePathBuffer = VirtualAllocEx(hTargetProcess, NULL, iRemoteMemSize, MEM_COMMIT, PAGE_READWRITE);
		if (pRemotePathBuffer == NULL) __leave;

		// Write to the string buffer
		if (!WriteProcessMemory(hTargetProcess, pRemotePathBuffer, szModuleName, iRemoteMemSize, NULL)) __leave;

		// Have the remote process call LoadLibrary for our dll
		PTHREAD_START_ROUTINE pThreadRtn = (PTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandleW(L"kernel32.dll"), "LoadLibraryW");
		hRemoteThread = CreateRemoteThread(hTargetProcess, NULL, 0, pThreadRtn, pRemotePathBuffer, 0, NULL);
		if (hRemoteThread == NULL) {
			DWORD dwError = GetLastError();
			(void)dwError;
			__leave;
		}
		WaitForSingleObject(hRemoteThread, INFINITE);


		// Note: a DWORD is only 32 bits, so it's not big enough to hold the address of our newly loaded dll
		DWORD injectedDllBase = 0;
		GetExitCodeThread(hRemoteThread, &injectedDllBase);


		// Use EnumProcessModules on the remote process to find the hmodule of our dll
		HANDLE hRemoteMod = NULL;
		HMODULE hMods[1024];
		DWORD cbNeeded;
		if (!EnumProcessModules(hTargetProcess, hMods, sizeof(hMods), &cbNeeded)) {
			__leave;
		}
		cbNeeded /= sizeof(HMODULE);
		for (DWORD i = 0; i < cbNeeded; ++i) {
			wchar_t szCurModName[LSTR];
			if (GetModuleFileNameEx(hTargetProcess, hMods[i], szCurModName, sizeof(szCurModName) / sizeof(wchar_t))) {
				if (wcscmp(szCurModName, szModuleName) == 0) {
					hRemoteMod = hMods[i];
					break;
				}
			}
		}
		if (hRemoteMod == NULL) __leave;


		// Cleanup our prep work
		VirtualFreeEx(hTargetProcess, pRemotePathBuffer, 0, MEM_RELEASE);
		CloseHandle(hRemoteThread);
		hRemoteThread = NULL;
		pRemotePathBuffer = NULL;
		if (injectedDllBase == NULL) __leave;


		// Copy data passed in from the injector to the remote process
		SharedData* pSharedData = (SharedData*)(lpParam);
		const size_t uSharedDataSize = sizeof(SharedData);
		if (pSharedData == nullptr) __leave;
		pRemoteSharedData = VirtualAllocEx(hTargetProcess, NULL, uSharedDataSize, MEM_COMMIT, PAGE_READWRITE);
		if (pRemoteSharedData == NULL) __leave;

		// Write to the remote data structure
		SIZE_T szBytesWritten = 0;
		if (!WriteProcessMemory(hTargetProcess, pRemoteSharedData, pSharedData, uSharedDataSize, &szBytesWritten)) __leave;

		// Get the address for our remote method
		INT_PTR remoteMethodAddress = INT_PTR(hRemoteMod) + methodDelta;

		// Run the remote method
		PTHREAD_START_ROUTINE pRemoteMethod = (PTHREAD_START_ROUTINE)remoteMethodAddress;
		hRemoteThread = CreateRemoteThread(hTargetProcess, NULL, 0, pRemoteMethod, pRemoteSharedData, 0, NULL);

		if (hRemoteThread == NULL) __leave;
		result = hRemoteThread;
		hRemoteThread = NULL;

	}
	__finally
	{
		if (hRemoteThread != NULL) CloseHandle(hRemoteThread);
		if (pRemotePathBuffer != NULL) VirtualFreeEx(hTargetProcess, pRemotePathBuffer, 0, MEM_RELEASE);
		if (hTargetProcess != NULL) CloseHandle(hTargetProcess);
	}

	return result;

}


// ** EjectLibrary	- Removes DLL from specified target process
extern "C" __declspec(dllexport) void EjectLibrary(DWORD dwTargetProcessID)
{
	HANDLE hTargetProcess = NULL;
	wchar_t szModuleName[MAX_PATH];
	LPVOID pRemotePathBuffer = NULL;
	HANDLE hRemoteThread = NULL;

	__try
	{
		hTargetProcess = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_VM_OPERATION | PROCESS_VM_WRITE, FALSE, dwTargetProcessID);
		if (hTargetProcess == NULL) __leave;

		// Get the name of this .dll and store that string in the remote process
		memset(szModuleName, 0, sizeof(wchar_t) * LSTR);
		GetModuleFileNameW(hThisModule, szModuleName, LSTR);
		szModuleName[LSTR - 1] = 0;

		// Allocate the string buffer in the remote process
		int iRemoteMemSize = sizeof(wchar_t) * (lstrlenW(szModuleName) + 1);
		pRemotePathBuffer = VirtualAllocEx(hTargetProcess, NULL, iRemoteMemSize, MEM_COMMIT, PAGE_READWRITE);
		if (pRemotePathBuffer == NULL) __leave;

		// Write to the string buffer
		if (!WriteProcessMemory(hTargetProcess, pRemotePathBuffer, szModuleName, iRemoteMemSize, NULL)) __leave;

		// Have the remote process call GetModuleHandle for our dll
		PTHREAD_START_ROUTINE pThreadRtn = (PTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandleW(L"kernel32.dll"), "GetModuleHandleW");
		hRemoteThread = CreateRemoteThread(hTargetProcess, NULL, 0, pThreadRtn, pRemotePathBuffer, 0, NULL);
		if (hRemoteThread == NULL) __leave;
		WaitForSingleObject(hRemoteThread, INFINITE);


		DWORD injectedDllBase = 0;
		GetExitCodeThread(hRemoteThread, &injectedDllBase);

		// Cleanup our prep work
		VirtualFreeEx(hTargetProcess, pRemotePathBuffer, 0, MEM_RELEASE);
		CloseHandle(hRemoteThread);
		hRemoteThread = NULL;
		pRemotePathBuffer = NULL;
		if (injectedDllBase == NULL) __leave;


		// Have the remote process call FreeLibrary for our dll
		pThreadRtn = (PTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandleW(L"kernel32.dll"), "FreeLibrary");
		hRemoteThread = CreateRemoteThread(hTargetProcess, NULL, 0, pThreadRtn, (LPVOID)injectedDllBase, 0, NULL);

	}
	__finally
	{
		if (hRemoteThread != NULL) CloseHandle(hRemoteThread);
		if (pRemotePathBuffer != NULL) VirtualFreeEx(hTargetProcess, pRemotePathBuffer, 0, MEM_RELEASE);
		if (hTargetProcess != NULL) CloseHandle(hTargetProcess);
	}

}


BOOL WINAPI DllMain(HANDLE hinstDLL, DWORD dwReason, LPVOID lpvReserved)
{
	// in dll main hThisModule variable is initialized
	if (dwReason == DLL_PROCESS_ATTACH) hThisModule = (HMODULE)hinstDLL;
	return TRUE;
}
