/*
 * 
 * @Author - Pause_009
 * Entry point for the console application.
 * 
 */

using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using System.Text.RegularExpressions;
using ScamConsole.cmds;

namespace ScamConsole
{
    class Program
    {
        
        static void Main(string[] args)
        {

            try
            {

                //Instanciate CMD Factory for running commands in .cfg file
                CMDFactory commands = new CMDFactory();

                //Get FileName of the app for looking up the .cfg file
                string fileName = System.AppDomain.CurrentDomain.FriendlyName;

                //Create full path to .cfg file
                string cfgPath = AppDomain.CurrentDomain.BaseDirectory + fileName + ".cfg";

                //Get lines from .cfg file
                IEnumerable<string> fileLines = File.ReadLines(cfgPath);

                //Loop through and process each line one-by-one
                foreach (string line in fileLines)
                {

                    ProcessLine(line, commands);
                    
                }

            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);
            }


        }
        
        static void ProcessLine(string line, CMDFactory commands)
        {
            //Check if line is a command by checking for parsing symbols
            if(line.StartsWith("{") && line.EndsWith("}") && line.Contains(":"))
            {
                //Regex for extracting command name
                string commName = Regex.Matches(line, @"\{(.+?)\:")
                                    .Cast<Match>()
                                    .Select(m => m.Groups[1].Value).FirstOrDefault();

                //Regex for extracting parameters
                string commParams = Regex.Matches(line, @"\:(.+?)\}")
                                    .Cast<Match>()
                                    .Select(m => m.Groups[1].Value).FirstOrDefault();

                //Create array of parameters by splitting the string by ','
                string[] paramSplit = commParams.Split(',');

                //Get command from CMDFactory
                ICommand scamCMD = commands.GetCMD(commName);

                //Run command
                scamCMD.Run(paramSplit);

            }
            else
            {
                //If line is not a command simply print out the string as is.
                Console.WriteLine(line);
            }

        }


    }
}
