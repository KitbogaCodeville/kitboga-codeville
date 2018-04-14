/*
 * 
 * Interface for creating commands to be run from .cfg files 
 * 
 * 
 */

namespace ScamConsole.cmds
{
    public interface ICommand
    {
        void Run(string[] cmdParams);
    }
}
