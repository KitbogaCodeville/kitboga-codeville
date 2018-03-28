::IMPORTANT - for this to work it must be placed in C:\Windows\System32
::            or somewhere that has been added to Path.
::
::            THE REAL TREE (tree.com) MUST ALSO BE RENAMED
::            TO realtree.com
::            Created by Discord user - sansfont

if "%~1"=="-FIXED_CTRL_C" (
   SHIFT
) ELSE (
   CALL <NUL %0 -FIXED_CTRL_C %*
   GOTO :EOF
)

@echo off
:dothetree
realtree.com
goto :dothetree
