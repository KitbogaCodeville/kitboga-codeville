; Originally authored by Nameless#7323/sgtqwerty and shared on the official Kitboga Discord
; This script grants the ability to type numbers either one-off, or as random other numbers

#SingleInstance force
#Persistent
#NoTrayIcon

; Will only run if Notepad is open, can be commented out to run on any program
#IfWinActive, ahk_exe notepad.exe

; 1 is off, 2 is minus-one, 3 is random
global MODE := 1

;(Ctrl + WindowsKey + Alt + Numpad1) Off
;(Ctrl + WindowsKey + Alt + Numpad2) Minus-One
;(Ctrl + WindowsKey + Alt + Numpad3) Random
;(Ctrl + WindowsKey + Alt + NumpadSub) Exit

#UseHook

!#^Numpad1:: ; Off
	MODE := 1
return

!#^Numpad2:: ; Minus-One
	MODE := 2
return

!#^Numpad3:: ; Random
	MODE := 3
return

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
if (MODE == 2) { ; Sends any number key pressed minus one
  if (A_ThisHotkey == 0) {
    send 9
  } else {
    send % A_ThisHotkey - 1
  }
} else if (MODE == 3) { ; Sends any random number key when any other number key is pressed
  Random, rand, 0, 9
  send %rand%
} else {
  send % A_ThisHotkey
}
return

;***** Hotkey to End script *****
!#^NumpadSub::ExitApp