; Originally authored by Nameless#7323/sgtqwerty and shared on the official Kitboga Discord
; Mode select and fat fingers added by DangerBK
; This script grants the ability to type numbers either one-off, random other numbers, or numbers close by at a random chance

#SingleInstance force
#Persistent
#NoTrayIcon

; Will only run if Notepad is open, can be commented out to run on any program
#IfWinActive, ahk_exe notepad.exe

; 1 is off, 2 is minus-one, 3 is random
global MODE := 1

;(Ctrl + WindowsKey + Alt + Numpad1) Off            (Returns normal numbers)
;(Ctrl + WindowsKey + Alt + Numpad2) Minus-One      (Returns the number minus 1)
;(Ctrl + WindowsKey + Alt + Numpad3) Random         (Returns a random number)
;(Ctrl + WindowsKey + Alt + Numpad4) Fat Finger     (Returns a random chance of the original number, or the number - 1, or the number + 1)
;(Ctrl + WindowsKey + Alt + NumpadSub) Exit         (Shuts down the script)

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

!#^Numpad4:: ; Fat Finger
	MODE := 4
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
} else if (MODE == 4) { ; Sends either the original number, the number -1, or the number plus one. Weighted to send the original key more often
  Random, rand, 1, 10
  if (rand < 2) {
    if (A_ThisHotkey == 1) {
      send 2
    } else if (A_ThisHotkey == 0) {
      send 9
    } else {
      send % A_ThisHotkey - 1
    }
  } else if (rand < 10) {
    send % A_ThisHotkey
  } else {
    if (A_ThisHotkey == 0) {
      send 9
    } else if (A_ThisHotkey == 9) {
      send 0
    } else {
      send % A_ThisHotkey + 1
    }
  }
} else {
  send % A_ThisHotkey
}
return

;***** Hotkey to End script *****
!#^NumpadSub::ExitApp