; Originally authored by Nameless#7323/sgtqwerty and shared on the official Kitboga Discord
; This script gives a 10% chance to cancel any typed key

#SingleInstance force
#Persistent
#NoTrayIcon

; get 10% chance to type backspace per typed key
loop {
    Input, L, L1 V
    Random, rand, 0, 9
    if(rand == 9) {
      sendinput {backspace}
    }
}