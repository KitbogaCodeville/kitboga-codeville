;This is the script that Kitboga wrote on March 25th to play around with telescammers using Notepad
;Some changes have been added such as the top metadata labels and ahk_exe calls

#SingleInstance force
#Persistent
#NoTrayIcon

global numpadHP := 255
global dmg := 2
global npX := 100
global npY := 100
global shake := 2

;Opens notepad and resizes
Run, notepad.exe
WinWait, ahk_exe notepad.exe
WinMove, ahk_exe notepad.exe, , npX, npY, 500, 400
WinSet, Transparent, 255, ahk_exe notepad.exe

;Prepares CheckNotepadHP to be called every 10ms
SetTimer, CheckNotepadHP, 10

;Shake window by a small amount
Shake() {
	Random, rX, -shake, shake
	Random, rY, -shake, shake
	npX := npX + rX
	npY := npY + rY
	WinMove, ahk_exe notepad.exe, , npX, npY
}

;Loop label to change transparency and shake window when HP is low enough
CheckNotepadHP:
	clipboard = ; Clears the clipboard to prevent paste https://autohotkey.com/docs/misc/Clipboard.htm
	WinSet, Transparent, %numpadHP%, ahk_exe notepad.exe

	if (numpadHP < 200) {
		Shake()
	}
return

;Increases transparency
DamageNotepad() {
	if (numpadHP - dmg >= 0) {
		numpadHP := numpadHP - dmg
	}
}

;Reduces transparency
HealNotepad() {
	if (numpadHP + dmg <= 255) {
		numpadHP := numpadHP + dmg
	}
}

~a:: DamageNotepad()
~+a:: DamageNotepad() ;+indicates shift key
~e:: DamageNotepad()
~+e:: DamageNotepad()
~i:: DamageNotepad()
~+i:: DamageNotepad()
~o:: DamageNotepad()
~+o:: DamageNotepad()
~u:: DamageNotepad()
~+u:: DamageNotepad()
~y:: DamageNotepad()
~+y:: DamageNotepad()
~Space:: DamageNotepad()

~Backspace:: HealNotepad()
