;Gravity/Physics for windows!!!
;
;Author: NiceShotToby
;Revision: 1.0
;Tested On: Windows 8
;
;Currently has Gravity, Bouncing, Friction and Throwing all adjustable.
;I did not implement the disabling maximizing but feel free to do so.
;Throwing works by taking the difference from when you press and release the mouse and
;setting the velocity to that, not perfect but anything better would likely involve complex
;management of the threads.
;
;****Suggested Settings******
;Realistic: 3 Gravity, -0.5 Bounce, 5 Bounce Limiter, 0.4 Friction
;TV Screensaver: 0 Gravity, -1 Bounce, 0 Bounce Limiter, 0 Friction (Toss away and enjoy)
;Helium Windows: -3 Gravity, -0.2 Bounce, 5 Bounce Limiter, 0.6 Friction 
;Concrete Windows: 10 Gravity, 0 Bounce, 0 Bounce Limiter, 0.8 Friction

;*****Gravity Acceleration Multiplier*****
;Positive makes windows accelerate down, negative makes them accelerate up, 0 is off
global GRAVITY := 0

;*****Bounce Multiplier*****
;Must be <= 0, 0 is no bounce, -1 is perfect elasticity
global BOUNCE := -1

;*****Bounce Limiter*****
;Must be >= 0, Anything below the specified velocity will not bounce/stick, 0 is off
;Adjust to prevent neverending bouncing 
global BOUNCE_VEL_LIMIT := 0

;*****Friction Multiplier*****
;The effect friction has on windows sliding on the bottom of the screen
;Positive values will slow down windows, negative values will speed up windows, 0 is off
global FRICTION := 0

;*****Throw Sensitivity*****
;Adjusts the sensitivity of thrown windows
;Larger numbers are more sensitive, 0 is off
global THROW_SENSITIVITY := 20

;*****Screen/Area Adjustments*****
;Adjustable value representing taskbar size
global MENU_OFFSET := 50

;Adjustable value to shrink the box windows will bounce off of
global BORDER := 0

;*Quit/Exit Hotkey located at bottom of file*

;************  END OF ADJUSTABLE VALUES  *****************

global TOP := (0 + BORDER)
global BOTTOM := (A_ScreenHeight - MENU_OFFSET - BORDER)
global LEFT := (0 + BORDER)
global RIGHT := (A_ScreenWidth - BORDER)

global XVelArray := {}
global YVelArray := {}

global DragStartX
global DragStartY
global DragStartTimeSec
global DragStartTimeMSec
global DragStartWin

SetBatchLines, -1
SetWinDelay, -1
SetTimer, Physics, 1
return

Physics: 
	WinGet, windows, List,,,""
	Loop %windows% {
		
		id := windows%A_Index%
		WinGetTitle wt, ahk_id %id%
		WinGetPos, Xpos, Ypos, WinWidth, WinHeight, %wt% ;Get window info
		
		if (!XVelArray.HasKey(wt)) { ;if window not in array 
			;add to x and y array with 0
			XVelArray[wt] := 0
			YVelArray[wt] := 0
		}

		if (Ypos + WinHeight) < BOTTOM { ;if not on ground
			;apply gravity to y velocity
			YVelArray[wt] := (YVelArray[wt] + GRAVITY) 
		}
		else { ; if on ground
			if XVelArray[wt] < 0 { ;if velocity neg
				if (-FRICTION < XVelArray[wt]) {
					XVelArray[wt] := 0
				}
				else {
					;add friction to x velocity
					XVelArray[wt] := (XVelArray[wt] + FRICTION)
				}
			}
			else if XVelArray[wt] > 0 { ;elseif velocity pos
				if (FRICTION > XVelArray[wt]) {
					XVelArray[wt] := 0
				}
				else {
					;subtract friction from x velocity
					XVelArray[wt] := (XVelArray[wt] - FRICTION)
				}			
			}
			else { ;if vel = 0
				;nothing
			}
		}
		if (YVelArray[wt] > 0) AND ((Ypos + WinHeight + YVelArray[wt]) >= BOTTOM) { ;if y vel is pos and within velocity from BOTTOM
			if (YVelArray[wt] < BOUNCE_VEL_LIMIT) { ;if y velocity < bounce limit
				;set y coord to BOTTOM
				WinMove,%wt%,, Xpos, (BOTTOM - WinHeight) 
				Ypos := (BOTTOM - WinHeight)
				;set y velocity to 0
				YVelArray[wt] := 0
			}
			else { ;else ( bounce condition)
				;set y coord to BOTTOM
				WinMove,%wt%,, Xpos, (BOTTOM - WinHeight)
				Ypos := (BOTTOM - WinHeight)	
				;set y velocity to BOUNCE * current velocity
				YVelArray[wt] := (YVelArray[wt] * BOUNCE)
			}
		}
		else if (YVelArray[wt] < 0) AND ((Ypos + YVelArray[wt]) <= TOP) { ;if y vel is neg and within velocity from TOP
			if (Abs(YVelArray[wt]) < BOUNCE_VEL_LIMIT) { ;if y velocity < bounce limit
				;set y coord to TOP
				WinMove,%wt%,, Xpos, TOP
				Ypos := TOP
				;set y velocity to GRAVITY
				YVelArray[wt] := GRAVITY
			}	
			else { ;else ( bounce condition)
				;set y coord to TOP
				WinMove,%wt%,, Xpos, TOP
				Ypos := TOP
				;set y velocity to BOUNCE * current velocity + GRAVITY
				YVelArray[wt] := (YVelArray[wt] * BOUNCE)
			}
		}
		else { ;else move in y dir
			;move in y direction by velocity
			WinMove,%wt%,, Xpos, (Ypos + YVelArray[wt])
			Ypos := (Ypos + YVelArray[wt])
		}
		if (XVelArray[wt] > 0) AND ((Xpos + WinWidth + XVelArray[wt]) >= RIGHT) { ;if x vel is pos and within velocity from RIGHT
			if (Abs(XVelArray[wt]) < BOUNCE_VEL_LIMIT) { ;if x velocity < bounce limit
				;set x coord to RIGHT
				WinMove,%wt%,, (RIGHT - WinWidth), Ypos
				;set x velocity to 0
				XVelArray[wt] := 0
			}
			else { ;else ( bounce condition)
				;set x coord to RIGHT
				WinMove,%wt%,, (RIGHT - WinWidth), Ypos
				;set x velocity to BOUNCE * current velocity
				XVelArray[wt] := (XVelArray[wt] * BOUNCE)
			}
		}
		else if (XVelArray[wt] < 0) AND ((Xpos + XVelArray[wt]) <= LEFT) { ;elseif x vel is neg and within velocity from LEFT
			if (Abs(XVelArray[wt]) < BOUNCE_VEL_LIMIT) { ;if x velocity < bounce limit
				;set x coord to LEFT
				WinMove,%wt%,, LEFT, Ypos
				;set x velocity to 0
				XVelArray[wt] := 0
			}	
			else { ;else ( bounce condition)
				;set x coord to LEFT
				WinMove,%wt%,, LEFT, Ypos
				;set x velocity to BOUNCE * current velocity
				XVelArray[wt] := (XVelArray[wt] * BOUNCE)
			}
		} 
		else { ;else move in x dir
			;move in x direction by velocity
			WinMove,%wt%,, (Xpos + XVelArray[wt]), Ypos
		}

	}
return

;Collect location, time and the window the mouse is over when clicked
~LButton::
	MouseGetPos, DragStartX, DragStartY, DragStartWin
	DragStartTimeSec := A_Sec
	DragStartTimeMSec := A_MSec
return

;Collect location and time when mouse is released, calculate velocity of window based on deltas
~LButton Up:: 
	WinGetTitle DragWin, ahk_id %DragStartWin%
	
	TimeElapsed := (((A_Sec - DragStartTimeSec) * 1000) + (A_MSec - DragStartTimeMSec))
	
	MouseGetPos, DragEndX, DragEndY
	Xdelta := (DragEndX - DragStartX)
	Ydelta := (DragEndY - DragStartY)

	XVelArray[DragWin] := ((Xdelta / TimeElapsed) * THROW_SENSITIVITY) ;set active window x velocity to difference between stored and cur x coord / time
	YVelArray[DragWin] := ((Ydelta / TimeElapsed) * THROW_SENSITIVITY) ;set active window y velocity to difference between stored and cur y coord / time
return

;***** Hotkey to End script *****
~Esc::ExitApp


