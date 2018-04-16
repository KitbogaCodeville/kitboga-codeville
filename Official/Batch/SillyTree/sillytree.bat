@ECHO OFF
TITLE SCANNING: JUST WAIT A MOMENT
MODE con: cols=50 lines=25
COLOR 0A

:LOOP
set /a rand=%random% %%7
ECHO.
ECHO                 SCANNING ROOT SYSTEM
ECHO                 ====================
ECHO                   LEAF YOUR MOUSE
ECHO.
ECHO.

TYPE tree%rand%.txt

set /a rand2=%random% %%1000 + 500
ping 192.168.1.1 -n 1 -w %rand2% > nul
CLS
GOTO LOOP