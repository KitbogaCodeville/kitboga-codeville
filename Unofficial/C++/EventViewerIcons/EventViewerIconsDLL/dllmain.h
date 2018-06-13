#pragma once

#include <tchar.h>

// ** SharedData - this data structure gets copied from our injector (EventViewerIcons.exe) to our injectee (mmc.exe aka Event Viewer) so that the DLL can access data set by the injector
struct SharedData
{
	HWND hwndWindow = NULL;
	TCHAR szPathIcon[260] = { '\0' };
};
