# Better Syskey #

This a small app designed to replace the stock windows syskey application.

This will not and cannot set the startup password or make any changes to the computer.

This mimics the look and the function of the real syskey application but when the startup password is submitted a window pops up and tells you that a tech support scammer is trying to lock you out of your computer along with a hacking kitty gif.

This also contains the ability annoy the scammer by pretending that the password and confirmation password typed do not match for X number of times.  The default is 0 (turned off), see the settings section below to enable it.

### How do I get this setup on my computer? ###

Well you are going to have to download the source code, open it in visual studio, and then build the project.

Next you will need to rename your existing syskey.exe to something else like syskey.orig or whatever you want.  You can find the exe in the folder: c:\windows\system32\

To rename the existing syskey.exe you may have to take ownership of the file and grant yourself all permissions on it first.

Next rename the BetterSyskey.exe that you built to syskey.exe and move it to the system32 folder.  

Next move the BetterSyskey.exe.config file to the same system32 folder.  Do not rename the config file.

Now you are all set to test it out.

To make sure that you don't accidently syskey yourself make sure that the steps above have been followed.

You can check if you are running the BetterSyskey application instead of the original by clicking on the update button then selecting the floppy disk password option, the BetterSyskey application will give you an error message about your computer needing a fixation.

### Settings ###

The setting(s) for this app are contained in the BetterSyskey.exe.config file.  This is an xml file that can be opened and edited using any quality text editor like notepad++.

The setting for faking the number of password mismatches is found under the <setting name="FakePasswordMismatchTimes"...> <value>0</value></setting> node.  The default is 0, set this to any number you want.  When the program loads it loads that value and it will tell the person entering a startup password X number of times that they mistyped the password and to do it again.