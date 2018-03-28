# IMPORTANT - for this to work it must be placed in C:\Windows\System32
#             or somewhere that has been added to Path.
#
#             THE REAL TREE (tree.com) MUST ALSO BE RENAMED
#             TO realtree.com
#             Created by Discord user - BenWirus

#$trees = @('tree-av.bat','tree-inf.bat','tree-bomb.bat');
$trees = @('tree-av.bat','tree-inf.bat');
$tree = $trees[(Get-Random -Maximum ([array]$trees).count)];
& $tree;
