This program modifies the "Event Viewer" process so that clicking on an "Error" or "Warning" changes it to say "Solved"
https://i.imgur.com/Rncq3HI.gifv

This program quietly runs in the background. When it finds an Event Viewer window, it performs a DLL injection that allows us to subclass the listview control.

Note: If event viewer is a 64 bit process, this program needs to be built as x64
