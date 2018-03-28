::IMPORTANT - for this to work it must be placed in C:\Windows\System32
::            or somewhere that has been added to Path.
::
::            THE REAL TREE (tree.com) MUST ALSO BE RENAMED
::            TO realtree.com
::            Created by Discord user - BenWirus

@echo off
Powershell.exe -executionpolicy remotesigned -File %windir%\System32\tree-picker.ps1
