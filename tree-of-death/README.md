# Tree of Death

Picks a randomly annoying tree script and runs them.

THE REAL TREE (`tree.com`) MUST ALSO BE RENAMED TO `realtree.com`.

Then copy these scripts to `%windir%\System32\`

----

## tree.bat:

A Batch shim that runs a PowerShell script that picks a random `tree-*.bat` file.

## tree-picker.ps1:

A PowerShell script that picks a random `tree-*.bat` file. This is called from `tree.bat`

## tree-av.bat:

Runs tree then displays an ASCII art image of a tree and output that shows that there is no malware on the PC.

## tree-inf.bat:

Runs tree forever...

## tree-bomb.bat:

Similar to `tree-inf.bat` but each process forks after tree is finished... basically a tree fork bomb. This might be dangerous without a way to kill all `cmd.exe` and `realtree.com` processes. This is not enabled by default and should stay that way until a gesture has been implemented that will kill all processes.
