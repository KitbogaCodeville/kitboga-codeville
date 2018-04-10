; Author: Gabriel Dube
; Create a floating red star that rotate around the cursor
;
; Enjoy @Kitboga


CreateSplash(x, y, w, h)
{
    Gui, +AlwaysOnTop +Disabled -SysMenu +HwndGuiHwnd -0xC00000 +E0x00080000 +Owner       ; Create a layered window (ie with transparency enabled) without any decoration
    Gui Color, 000000, f50000                                                             ; Set the background color to a solid color
    DllCall("SetLayeredWindowAttributes", UInt, GuiHwnd, UInt, 0, UChar, 0, UInt, 0x1)    ; Set the background color to be treated as transparent
    Gui, Add, Picture, x%x% y%y% w%w% h%h%, star.png                                      ; Add our gif (which should be next to the source file)
    Gui, Show,
    return GuiHwnd                                                                        ; Return the window handle
}

; Create the splash icon
handle := CreateSplash(0, 0, 70, 70)   ; Create the star out of screen

; Mouvement setup
xtime := 0                       ; Time            
time_dilatation := 0.04          ; How fast the star will move
radius := 100                    ; How far away from the cursor the star will rotate

; Static offset that center the splash on the mouse
yoffset := -40
xoffset := -30

; Make sure that MouseGetPos use screen coordinate (fix some funky results)
CoordMode, Mouse, Screen

Loop,
{
    ; get the cursor position
    MouseGetPos, mx, my 
    xtime++

    ; Move the star around the cursor (there's probably a simplier way)
    DllCall("SetWindowPos"
    , "UInt", handle ;handle
    , "UInt", 0 ;HWND_TOP
    , "Int", xoffset + mx + (sin(xtime*time_dilatation)*radius) ;x
    , "Int", yoffset + my + (cos(xtime*time_dilatation)*radius) ;y
    , "Int", 0 ;width
    , "Int", 0 ;height
    , "UInt", 0x4000|0x0001) ;SWP_ASYNCWINDOWPOS|SWP_NOSIZE

    ; set the smallest timestep possible (ensure a smooth animation)
    Sleep, 1
}
