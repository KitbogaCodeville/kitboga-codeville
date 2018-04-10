using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;

namespace NewTree
{
    class NewTree
    {
        // Fork Bomb disabled by default for safety
        static bool DisableForkBomb = true;

        static void Main(string[] args)
        {


            // Allow us to trap Ctrl-C
            Console.TreatControlCAsInput = true;
            Console.CancelKeyPress += ConsoleOnCancelKeyPress;
            Console.Title = "TREE";

            // Slightly weighted towards the Antiwirus
            Random rng = new Random();
            int diceMax = 5;
            int diceRoll = rng.Next(1, diceMax);
            switch (diceRoll)
            {
                case 1:
                    TreeForkBomb();
                    break;
                case 2:
                    NeverendingTree();
                    break;
                case 3:
                    TreeAntiwirus();
                    break;
                default:
                    TreeAntiwirus();
                    break;

            }
        }

        static Process RunRealTree(bool ShellExecute = false, bool WaitExit = true)
        {
            // If the real tree.com has been renamed, and use that, otherwise just return
            if (File.Exists(Environment.ExpandEnvironmentVariables("%windir%\\System32\\realtree.com")))
            {
                Process TreeProcess = new Process();
                TreeProcess.StartInfo.FileName = Environment.ExpandEnvironmentVariables("%windir%\\System32\\realtree.com");
                TreeProcess.StartInfo.Arguments = "\\"; // Made sure it runs tree from drive root
                TreeProcess.StartInfo.CreateNoWindow = false;
                TreeProcess.StartInfo.UseShellExecute = ShellExecute;
                TreeProcess.Start();
 
                if (WaitExit)
                {
                    TreeProcess.WaitForExit();
                }

                return TreeProcess;
            }
            return null;
        }

        static void TreeForkBomb()
        {
            Console.Title = "SO MANY TREES";

            // If the fork bomb function is disabled, just call the Antiwirus and return
            if (DisableForkBomb)
            {
                TreeAntiwirus();
                return;
            }
            // Seems like 12 is the sweet spot on my PC, possibly tweak it if the VM is less powerful
            int ForkProcessMax = 12;

            List<Process> ForkProcessList = new List<Process>();

            // This will loop until a key is pressed, then it will check it against the key listed
            // Note that it can be hard to focus the main window anyway, and closing it might be easier
            while (!(Console.KeyAvailable && Console.ReadKey(true).Key == ConsoleKey.Escape))
            {
                if (ForkProcessList.Count < ForkProcessMax)
                {
                    ForkProcessList.Add(RunRealTree(true, false));
                }

                foreach (Process p in ForkProcessList.Reverse<Process>())
                {
                    if (p.HasExited)
                    {
                        ForkProcessList.Remove(p);
                    }
                }
            }
        }

        static void NeverendingTree()
        {
            Console.Title = "TREE FOREVER";
            while (true)
            {
                RunRealTree();
            }
        }

        static void TreeAntiwirus()
        {

            // Lovely artwork courtesy of Discord #codeville
            string TreeOutput = @"
                              # #### ####
                            ### \/#^|### ^|/####
                           ##\/#/ \^|^|/##/_/##/_#
                         ###  \/###^|/ \/ # ###
                       ##_\_#\_\## ^| #/###_/_####
                      ## #### # \ #^| /  #### ##/##
                       __#_--###`  ^|{,###---###-~
                                 \ }{
                                  }}{
                                  }}{
                                  {{}
                            , -^=-~{ .-^- _
                                  `}
                                   {

            ---------------------------------
             ==TREE ANTIWIRUS SCAN RESULTS== 
            ---------------------------------
              NO VIRUSES FOUND
              NO AD-AWARE FOUND
              NO MALWARE FOUND
              NO TROJANS FOUND
              RETICULATING SPLINES FOUND
              NO ROOTKITS FOUND
              NO UPDOGS FOUND
              NO RANDSOMWARE FOUND    
              NO HACKERS FOUND
            ---------------------------------
            ";

            Console.Title = "TREE ANTIWIRUS SCAN";
            RunRealTree();
            Console.WriteLine(TreeOutput);
        }

        private static void ConsoleOnCancelKeyPress(object sender, ConsoleCancelEventArgs consoleCancelEventArgs)
        {
            consoleCancelEventArgs.Cancel = true;
        }
    }
}
