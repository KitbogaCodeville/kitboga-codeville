/*
 * 
 * Sleep command
 * {Sleep:2000}
 * 
 */

using System.Threading;

namespace ScamConsole.cmds
{
    class SleepCMD : ICommand
    {
        
        public void Run(string[] cmdParams)
        {
            //Pause console app execution to simulate app load
            Thread.Sleep(int.Parse(cmdParams[0]));

        }

    }
}
