using System;
using System.Collections.Generic;

namespace ScamConsole.cmds
{
    public class CMDFactory
    {
        //Dictionart used to look up the ICommand classes by the command name
        private Dictionary<string, ICommand> cmdMap =  new Dictionary<string, ICommand>();

        public CMDFactory()
        {
            //All commands should be added to the factory here
            cmdMap.Add("Sleep", new SleepCMD());
        }

        //Method to get the ICommand by name
        public ICommand GetCMD(string cmdName)
        {
            //Check if the Dictionary contains the command name, if not throw an exception.
            if (cmdMap.ContainsKey(cmdName))
            {
                //Lookup ICommand from Dictionary
                return cmdMap[cmdName];
            }
            else
            {
                throw new Exception("Command ["+cmdName+"] was not found in CMDFactory");
            }
        }

    }
}
