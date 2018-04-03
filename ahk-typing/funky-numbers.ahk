; Originally authored by Nameless#7323/sgtqwerty and shared on the official Kitboga Discord
; This script grants the ability to type numbers either one-off, or as random other numbers

#SingleInstance force
#Persistent
#NoTrayIcon

; Will only run if Notepad is open, can be commented out to run on any program
#IfWinActive, ahk_exe notepad.exe


; Sends any number key pressed minus one
#UseHook
; 1::
; 2::
; 3::
; 4::
; 5::
; 6::
; 7::
; 8::
; 9::
;   send % A_ThisHotkey - 1
; return

; Sends any random number key when any other number key is pressed
1::
2::
3::
4::
5::
6::
7::
8::
9::
0::
  Random, rand, 0, 9
  send %rand%
return