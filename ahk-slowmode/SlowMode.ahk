global Delay := 600 ; Adjust this to the amount of permitted time between each key stroke
#UseHook On ; Prevents sending keys from triggering other hot keys

; Enable slow mode with alt + s
!s:: 
global LastTime := A_TickCount

;For each letter (a-zA-Z), apply slow key
Loop 26 {
	Hotkey, % Chr(A_Index+96), SlowKey
	Hotkey, % "+"Chr(A_Index+96), SlowKey
}
Return

; Disable slow mode (enable fast mode) alt + f
!f::

;For each letter (a-zA-Z), apply doNothing
Loop 26 {
	Hotkey, % Chr(A_Index+96), DoNothing
	Hotkey, % "+"Chr(A_Index+96), DoNothing
}
Return

;Slow key detects the time between the last time you pressed a key 
;If on, then it doesn't let you type another until Speed ms have passed.
;If you type a key to fast the timer resets.
SlowKey()
{
	Delta := A_TickCount - LastTime
	LastTime := A_TickCount
	if (Delay < Delta) {
		Key := A_ThisHotkey
		Send, %Key%
	}
	Return
}
;This method does nothing and is used to unassign hotkeys
DoNothing()
{
	Key := A_ThisHotkey
	Send, %Key%
	Return
}

!Esc::ExitApp
