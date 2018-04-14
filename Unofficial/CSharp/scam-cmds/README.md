# scam-cmds

C# console application which can be setup to display pre-configfured text in the console by configuring the .cfg file. 

The .cfg file must be in the same directory as the console app and also have the same name as the console app (ex.tree.com.cfg). Examples for tree and netstat are included in the project.

The configuration files also support running commands, current the only commands implemented is "Sleep" which pauses writing to the console to simulate loading. A framework is setup so additional commands can be added to the app in the future.


##### Current Commands:
1. {Sleep:2000} - Parameter 0 = time to wait in milliseconds



##### NETSTAT.EXE.cfg example
```
Active Connections

  Proto  Local Address          Foreign Address        State
{Sleep:2000}
  TCP    10.0.2.15:49390        64.4.54.254:https      HACKER_BLOCKED
{Sleep:2000}
  TCP    10.0.2.15:49399        chi28s15-in-f3:https   HACKER_BLOCKED

All hackers blocked, disregard any messages below, they are false positives...