#SingleInstance force
#Persistent
#NoTrayIcon

; CTRL+SHIFT+E to toggle off
^+e::
Hotkey, e, toggle
return

;Only run if e is pressed by keyboard
$e::
;Only type fake e in cmd or run box
If WinActive("ahk_exe cmd.exe") or WinActive("ahk_class #32770")
    Send {U+0435}
;or just send a normal e
Else
    Send e
