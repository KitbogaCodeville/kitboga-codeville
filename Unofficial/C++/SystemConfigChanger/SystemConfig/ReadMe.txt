========================================================================
    CONSOLE APPLICATION : SystemConfig Project Overview
========================================================================

This program changes the text of listview items in the System Configuration dialog (msconfig.exe) based on entries in a text file.
See https://i.imgur.com/uxoduGg.gifv for an example of it in action.
The text file should be in the following format, quotation marks included:

"Stopped" "Bogus Status"
"Microsoft Corporation" "Bogus Company"

The text will update as soon as the find & replace text file is saved.

*** Currently only finds & replaces full strings, no substrings

*** Note: If msconfig is a 64 bit process, this program also needs to be compiled as x64.






AppWizard has created this SystemConfig application for you.

This file contains a summary of what you will find in each of the files that
make up your SystemConfig application.


SystemConfig.vcxproj
    This is the main project file for VC++ projects generated using an Application Wizard.
    It contains information about the version of Visual C++ that generated the file, and
    information about the platforms, configurations, and project features selected with the
    Application Wizard.

SystemConfig.vcxproj.filters
    This is the filters file for VC++ projects generated using an Application Wizard. 
    It contains information about the association between the files in your project 
    and the filters. This association is used in the IDE to show grouping of files with
    similar extensions under a specific node (for e.g. ".cpp" files are associated with the
    "Source Files" filter).

SystemConfig.cpp
    This is the main application source file.

/////////////////////////////////////////////////////////////////////////////
Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named SystemConfig.pch and a precompiled types file named StdAfx.obj.

/////////////////////////////////////////////////////////////////////////////
Other notes:

AppWizard uses "TODO:" comments to indicate parts of the source code you
should add to or customize.

/////////////////////////////////////////////////////////////////////////////
