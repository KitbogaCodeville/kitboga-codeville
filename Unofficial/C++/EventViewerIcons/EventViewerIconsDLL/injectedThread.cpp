#include "stdafx.h"
#include <CommCtrl.h>
#include <windows.h>
#include <map>
#include <process.h>
#include <string>
#include <set>
#include <sstream>
#include <tchar.h>

#include "dllmain.h"

#define LSTR	260
using namespace std;

// The target process uses a "virtual" style listview control - https://msdn.microsoft.com/en-us/library/windows/desktop/bb774735(v=vs.85).aspx#Virtual_ListView_Style
// This means that the listview item data (item text, icon index, etc.) isn't stored in the item, but rather in some custom data structure within the remote process.
// The implication is that most data-altering messages (like LVM_SETITEMTEXT) aren't supported.
//
// Whenever the listview control needs to access the item data (so it knows what to draw for the item text, for example), it sends a LVN_GETDISPINFO notification.
// The remote process then handles the LVN_GETDISPINFO notification, and responds with the requested data.
//
// This means we can intercept the LVN_GETDISPINFO notification and respond with our own data to change how the listview items appear.

// ** NewWndProc - Subclassed window procedure for the listview control we're trying to modify
LRESULT CALLBACK NewWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	WNDPROC oldWndProc = WNDPROC(GetProp(hwnd, TEXT("OldWndProc")));
	switch (uMsg) {

		// Clean up on close...
		case WM_DESTROY:
			RemoveProp(hwnd, TEXT("OldWndProc"));
			SetWindowLongPtr(hwnd, GWLP_WNDPROC, LONG_PTR(oldWndProc));
			break;


		// Handle click events...
		case WM_NOTIFY: {
			map<int, wstring>* pMap = (map<int, wstring>*)GetProp(hwnd, TEXT("map"));

			NMHDR* pHdr = (NMHDR*)lParam;
			switch (pHdr->code) {


				// Intercept the attempt to sort a column and don't do anything (this prevents a crash).
				// Virtual style listviews don't support LVM_SORTITEMS so we can't sort the listview ourselves.
				case LVN_COLUMNCLICK:
					return 0;


				// Intercept the attempt to retrieve listview item data used for drawing and sorting
				case LVN_GETDISPINFO: {
					NMLVDISPINFO* plvdi = (NMLVDISPINFO*)lParam;

					// Allow the remote program handle the event normally, so it doesn't get confused and crash
					LRESULT result = CallWindowProc(oldWndProc, hwnd, uMsg, wParam, lParam);

					// Change the image icon for every listview item added to our map
					if (plvdi->item.mask & LVIF_IMAGE) {
						if (pMap != nullptr) {
							map<int, wstring>::iterator itMap = pMap->find(plvdi->item.iItem);
							if (itMap != pMap->end()) {
								// An item in the map matches the listview item index
								int iCheckIndex = int(GetProp(hwnd, TEXT("check")));
								plvdi->item.iImage = iCheckIndex;
							}
						}
					}

					// Change the item text for every listview item added to our map
					if ((plvdi->item.iSubItem == 0) && ((plvdi->item.mask & LVIF_TEXT) != 0)) {
						if (pMap != nullptr) {
							map<int, wstring>::iterator itMap = pMap->find(plvdi->item.iItem);
							if (itMap != pMap->end()) {
								// An item in the map matches the listview item index
								wcscpy_s(plvdi->item.pszText, LSTR, itMap->second.c_str());
								plvdi->item.cchTextMax = LSTR;
							}
						}
					}

					return result;

				} break;


				// Intercept item clicks
				case NM_CLICK: {
					LPNMITEMACTIVATE pItem = (LPNMITEMACTIVATE)lParam;
					if (pItem->iItem >= 0) {
						// Add the clicked item to the list of "Solved" items
						if (pMap != nullptr) pMap->insert(pair<int, wstring>(pItem->iItem, TEXT("Solved")));
						ListView_RedrawItems(pHdr->hwndFrom, pItem->iItem, pItem->iItem);
					}
				} break;
			}
		}
	}


	if (oldWndProc != NULL) return CallWindowProc(oldWndProc, hwnd, uMsg, wParam, lParam);
	return FALSE;
}

// ** EnumChildProc - Called for each child window. If the child window is a listview control, its hwnd is added to the set pointed to by lParam
//					* hwnd is the child control window handle
//					* lParam is a pointer to a set<HWND>
BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lParam)
{
	set<HWND>* psWindowHandles = (set<HWND>*)(lParam);
	if (psWindowHandles == nullptr) return FALSE;

	// Ignore hidden controls
	if (IsWindowVisible(hwnd) == FALSE) return TRUE;

	// Check the current hwnd type
	TCHAR szClassName[LSTR];
	if (GetClassName(hwnd, szClassName, LSTR) > 0) {

		// If hwnd is a List View control...
		wstring sListView(WC_LISTVIEW);
		wstring sClassName(szClassName);
		if (sClassName.find(sListView) != wstring::npos) { // Note: Spy++ tells us that the event viewer's listview control has the class name "WindowsForms10.SysListView32" so we can't simply compare the class name with WC_LISTVIEW
			psWindowHandles->insert(hwnd);
		}
	}

	return TRUE;
}


// ** WorkingThreadMain - This is the main loop for our injected DLL.
//						* lpParam is a pointer to a SharedData structure that was copied over from the injector process
void WorkingThreadMain(LPVOID lpParam)
{
	// Get a handle to the "Event Viewer" window
	SharedData* pSharedData = (SharedData*)(lpParam);
	if (pSharedData == nullptr) return;
	HWND hwndRemoteWindow = pSharedData->hwndWindow;

	map<int, wstring> mapItemData; // First value is the listview item index to change. Second value is the string to display for the changed item
	set<HWND> sHandlesProcessed; // Keep track of handles to child windows we've already processed. We won't want to subclass the same control twice

	while (IsWindow(hwndRemoteWindow)) {
		
		// Get a set of all listview control handles
		set<HWND> sHandlesToProcess;
		EnumChildWindows(hwndRemoteWindow, EnumChildProc, LPARAM(&sHandlesToProcess));


		for (set<HWND>::iterator iterControl = sHandlesToProcess.begin(); iterControl != sHandlesToProcess.end(); ++iterControl) {
			if (sHandlesProcessed.find(*iterControl) == sHandlesProcessed.end()) { // Only process controls that we haven't already modified


				// Subclass the listview control
				HWND hwndSubclass = GetParent(*iterControl);
				LONG_PTR OldWndProc = GetWindowLongPtr(hwndSubclass, GWLP_WNDPROC);
				SetProp(hwndSubclass, TEXT("OldWndProc"), HANDLE(OldWndProc));
				SetWindowLongPtr(hwndSubclass, GWLP_WNDPROC, (LONG_PTR)NewWndProc);
				SetProp(hwndSubclass, TEXT("map"), HANDLE(&mapItemData)); // Gives us access to the map from NewWndProc
				
				// Add our check icon to the listview's icon imagelist
				HIMAGELIST hImageList = ListView_GetImageList(*iterControl, LVSIL_SMALL);
				if (hImageList != NULL) {

					// Load our own icons
					HBITMAP hbmCheck = (HBITMAP)LoadImage(NULL, pSharedData->szPathIcon, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
					int iCheckIndex = ImageList_AddMasked(hImageList, hbmCheck, RGB(255, 255, 255));
					DeleteObject(hbmCheck);
					SetProp(hwndSubclass, TEXT("check"), HANDLE(iCheckIndex)); // Gives us access to the image index from NewWndProc
				}
				

				// Mark this window as processed so we don't keep changing it
				sHandlesProcessed.insert(*iterControl);
			}
		}

		Sleep(1000);	// Just wait a moment
	}
}

// ** WorkingThread - Entry point for working thread.
//					* lpParam is a pointer to a SharedData object
unsigned int __stdcall WorkingThread(LPVOID lpParam)
{
	__try
	{
		WorkingThreadMain(lpParam);
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
	}
	return 0;
}

// ** RemoteMethod	- This is the entry point of the remote thread, called by InjectAndStartThread
//					*  lpParam is a pointer to a SharedData object that was created in InjectAndStartThread
extern "C" __declspec(dllexport) unsigned int __stdcall RemoteMethod(LPVOID lpParam)
{

	HANDLE hAnotherThread = (HANDLE)_beginthreadex(NULL, 0, WorkingThread, lpParam, 0, NULL);
	WaitForSingleObject(hAnotherThread, INFINITE);
	CloseHandle(hAnotherThread);

	return 0;
}
