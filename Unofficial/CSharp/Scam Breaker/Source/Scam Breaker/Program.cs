using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using NotepadBreaker.Client;

namespace Scam_Breaker
{
    public class Program
    {
        [DllImport("user32.dll")]
        private static extern int GetWindowText(IntPtr hWnd, StringBuilder text, int count);

        [DllImport("user32.dll")]
        private static extern IntPtr GetMenu(IntPtr hWnd);

        [DllImport("user32.dll")]
        private static extern bool InsertMenu(IntPtr hMenu, Int32 wPosition, Int32 wFlags, Int32 wIDNewItem, string lpNewItem);

        [DllImport("user32.dll")]
        private static extern int GetMenuItemCount(IntPtr hMenu);

        [DllImport("user32.dll")]
        private static extern bool DrawMenuBar(IntPtr hWnd);

        [DllImport("user32.dll", SetLastError = true)]
        private static extern IntPtr FindWindowEx(IntPtr hwndParent,
            IntPtr hwndChildAfter, string lpszClass, IntPtr lpszWindow);

        [DllImport("user32.dll", EntryPoint = "SendMessage")]
        private static extern IntPtr SendMessageGetTextW(IntPtr hWnd,
            uint msg, UIntPtr wParam, StringBuilder lParam);

        [DllImport("User32.dll", EntryPoint = "SendMessage")]
        private extern static int SendMessageGetTextLength(IntPtr hWnd,
            int msg, IntPtr wParam, IntPtr lParam);

        [DllImport("user32.dll")]
        [return: MarshalAs(UnmanagedType.Bool)]
        private static extern bool SetForegroundWindow(IntPtr hWnd);

        [DllImport("user32.dll")]
        private static extern IntPtr SetWinEventHook(uint eventMin, uint eventMax, IntPtr
           hmodWinEventProc, WinEventDelegate lpfnWinEventProc, uint idProcess,
           uint idThread, uint dwFlags);

        [DllImport("User32.dll", CharSet = CharSet.Unicode)]
        public static extern int MessageBox(IntPtr h, string m, string c, int type);

        [DllImport("user32.dll")]
        private static extern bool UnhookWinEvent(IntPtr hWinEventHook);

        public delegate void WinEventDelegate(IntPtr hWinEventHook, uint eventType,
                      IntPtr hwnd, int idObject, int idChild, uint dwEventThread, uint dwmsEventTime);

        [DllImport("user32.dll", SetLastError = true)]
        private static extern IntPtr SetParent(IntPtr hWndChild, IntPtr hWndNewParent);

        //Sets window attributes
        [DllImport("USER32.DLL")]
        public static extern int SetWindowLong(IntPtr hWnd, int nIndex, int dwNewLong);

        [DllImport("user32.dll", SetLastError = true, CharSet = CharSet.Auto)]
        public static extern bool SetWindowText(IntPtr hwnd, String lpString);

        private const int MF_BYCOMMAND = 0x00000000;
        public const int SC_CLOSE = 0xF060;
        public const int SC_MINIMIZE = 0xF020;
        public const int SC_MAXIMIZE = 0xF030;

        [DllImport("user32.dll")]
        private static extern IntPtr GetSystemMenu(IntPtr hWnd, bool bRevert);

        [DllImport("user32.dll")]
        private static extern bool DeleteMenu(IntPtr hMenu, uint uPosition, uint uFlags);

        [DllImport("user32.dll", EntryPoint = "ShowWindow", SetLastError = true)]
        private static extern bool ShowWindow(IntPtr hWnd, int nCmdShow);

        public static WinEventDelegate procDelegate;

        public static IntPtr hhook;

        private const uint EVENT_OBJECT_INVOKED = 0x8013;
        private const uint WINEVENT_OUTOFCONTEXT = 0;
        private const Int32 _CustomMenuID = 1000;
        private const Int32 MF_BYPOSITION = 0x400;
        private const Int32 WM_SYSCOMMAND = 0x112;

        private const int SW_HIDE = 0;

        private const int SW_RESTORE = 9;

        public static Process ThisProcess;
        public static Configuration config;
        public static bool NeedsRefresh = false;

        public class Configuration
        {
            public bool TaskManagerKiller = false;
            public bool ForceShutdown = false;
            public bool HookToNotepad = true;
            public bool eventLogClear = false;
            public int Interval = 500;
            public TaskManagerMessage _TaskManagerMessage;
            public NotepadRows Rows = new NotepadRows() { rows = new List<NotepadMessage>() { new NotepadMessage() { RowMessage = "Scam breaker By @0x000T" } } };
            public List<NotepadKeyword> NotepadKeywords = new List<NotepadKeyword>() { new NotepadKeyword() { Keyword = "Customer", message = new TaskManagerMessage() { Message = "Scammer!", Title = "Scammer! xD" } } };
        }

        public class TaskManagerMessage
        {
            public string Message;
            public string Title;
        }

        public class NotepadRows
        {
            public List<NotepadMessage> rows = new List<NotepadMessage>();
        }

        public class NotepadMessage
        {
            public string RowMessage;
        }

        public class NotepadKeyword
        {
            public string Keyword;
            public bool Caps_Sensitive = false;
            public TaskManagerMessage message;
        }

        private static void Main(string[] args)
        {
            CreateConfigFile();
            ReadConfig();
            CreateWatcher();
            AppDomain.CurrentDomain.ProcessExit += new EventHandler(CurrentDomain_ProcessExit);
            Task.Run(async () => await PerformActions());
            ThisProcess = Process.GetCurrentProcess();
            Console.WriteLine("hey");
        wait:
            IdefiniteAsync().Wait();
            goto wait;
        }

        public static async Task IdefiniteAsync()
        {
            await Task.Delay(-1);
        }

        public static void CreateWatcher()
        {
            var watch = new FileSystemWatcher();
            watch.Path = @"C:\Windows";
            watch.NotifyFilter = NotifyFilters.LastWrite;
            watch.Changed += new FileSystemEventHandler(OnChanged);
            watch.EnableRaisingEvents = true;
        }

        public static List<IntPtr> handlesNp = new List<IntPtr>();
        public static Dictionary<IntPtr, IntPtr> handlesNpEdit = new Dictionary<IntPtr, IntPtr>();

        public static string getNotepadText(IntPtr npHandle)
        {
            return GetWindowText(FindWindowEx(npHandle, IntPtr.Zero, "Edit", IntPtr.Zero));
        }

        private static bool IsValidJson(string strInput)
        {
            strInput = strInput.Trim();
            if ((strInput.StartsWith("{") && strInput.EndsWith("}")) || //For object
                (strInput.StartsWith("[") && strInput.EndsWith("]"))) //For array
            {
                try
                {
                    var obj = JToken.Parse(strInput);
                    return true;
                }
                catch (JsonReaderException jex)
                {
                    //Exception in parsing json
                    Console.WriteLine(jex.Message);
                    return false;
                }
                catch (Exception ex) //some other exception
                {
                    Console.WriteLine(ex.ToString());
                    return false;
                }
            }
            else
            {
                return false;
            }
        }

        public static List<IntPtr> ShownMsg = new List<IntPtr>();

        public static bool IsConfig(IntPtr Handle)
        {
            return GetProcessByHandle(Handle).MainWindowTitle.Contains("Breaker_config.json");
        }

        public static void HookNotepad(List<string> Names)
        {
            handlesNp.Clear();
            handlesNpEdit.Clear();
            ShownMsg.Clear();

            handlesNp = Process.GetProcessesByName("notepad").Select(x => x.MainWindowHandle).ToList();

            foreach (var npHandle in handlesNp.Select((x, i) => new { Handle = x, Index = i }))
            {
                handlesNpEdit.Add(npHandle.Handle, FindWindowEx(npHandle.Handle, IntPtr.Zero, "Edit", IntPtr.Zero));
                var sysMenu = GetMenu(npHandle.Handle);

                if (config != null)
                {
                    var text = GetWindowText(FindWindowEx(npHandle.Handle, IntPtr.Zero, "Edit", IntPtr.Zero));
                    foreach (NotepadKeyword key in config.NotepadKeywords)
                    {
                        if (key.Caps_Sensitive)
                        {
                            if (text.Contains(key.Keyword))
                            {
                                if (!IsConfig(npHandle.Handle) && !ShownMsg.Exists(x => x == npHandle.Handle))
                                {
                                    ShownMsg.Add(npHandle.Handle);
                                    {
                                        try
                                        {
                                            SetWindowText(npHandle.Handle, "REKT by Scam Breaker!");
                                            GetProcessByHandle(npHandle.Handle).SetText(key.message.Message);
                                            {
                                                SetWindowLong(handlesNpEdit.ElementAt(npHandle.Index).Value, 0, 0);
                                            }
                                            IntPtr menu = GetSystemMenu(npHandle.Handle, false);
                                            if (menu != IntPtr.Zero)
                                            {
                                                DeleteMenu(menu, SC_CLOSE, MF_BYCOMMAND);
                                                DeleteMenu(menu, SC_MAXIMIZE, MF_BYCOMMAND);
                                                DeleteMenu(menu, SC_MINIMIZE, MF_BYCOMMAND);
                                            }
                                        }
                                        catch
                                        {
                                        }
                                    }
                                }
                            }
                        }
                        else if (!key.Caps_Sensitive)
                        {
                            if (text.ToLower().Contains(key.Keyword.ToLower()))
                            {
                                if (!IsConfig(npHandle.Handle) && !ShownMsg.Exists(x => x == npHandle.Handle))
                                {
                                    ShownMsg.Add(npHandle.Handle);
                                    {
                                        try
                                        {
                                            SetWindowText(npHandle.Handle, "REKT by Scam Breaker!");
                                            GetProcessByHandle(npHandle.Handle).SetText(key.message.Message);
                                            {
                                                SetWindowLong(handlesNpEdit.ElementAt(npHandle.Index).Value, 0, 0);
                                            }
                                            IntPtr menu = GetSystemMenu(npHandle.Handle, false);
                                            if (menu != IntPtr.Zero)
                                            {
                                                DeleteMenu(menu, SC_CLOSE, MF_BYCOMMAND);
                                                DeleteMenu(menu, SC_MAXIMIZE, MF_BYCOMMAND);
                                                DeleteMenu(menu, SC_MINIMIZE, MF_BYCOMMAND);
                                            }
                                        }
                                        catch
                                        {
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                if (GetMenuItemCount(sysMenu) < 6)
                {
                    int total = GetMenuItemCount(sysMenu);
                    for (int i = 0; i < Names.Count; i++)
                    {
                        InsertMenu(sysMenu, total + i, MF_BYPOSITION, _CustomMenuID, Names[i]);
                    }

                    DrawMenuBar(npHandle.Handle);
                }
            }

            hhook = SetWinEventHook(EVENT_OBJECT_INVOKED, EVENT_OBJECT_INVOKED, IntPtr.Zero,
                    procDelegate, 0, 0, WINEVENT_OUTOFCONTEXT);
        }

        public static Process GetProcessByHandle(IntPtr handle)
        {
            try
            {
                return Process.GetProcesses().Where(x => x.MainWindowHandle == handle).First();
            }
            catch
            {
                return null;
            }
        }

        public static void DeleteEventLogs()
        {
            foreach (var eventLog in EventLog.GetEventLogs())
            {
                eventLog.Clear();
                eventLog.Dispose();
            }
        }

        public static string GetWindowText(IntPtr hwnd)
        {
            int len = SendMessageGetTextLength(hwnd, 14, IntPtr.Zero, IntPtr.Zero) + 1;
            StringBuilder sb = new StringBuilder(len);
            SendMessageGetTextW(hwnd, 13, new UIntPtr((uint)len), sb);
            return sb.ToString();
        }

        private static async void OnChanged(object source, FileSystemEventArgs e)
        {
            FileSystemWatcher fsw_ = (FileSystemWatcher)source;

            try
            {
                fsw_.EnableRaisingEvents = false;
                if (e.Name == "Breaker_config.json")
                {
                    await Task.Run(() => ReadConfig());
                    Console.WriteLine("Config Refreshed!");
                }
            }
            finally
            {
                fsw_.EnableRaisingEvents = true;
            }
        }

        public static bool IsFileLocked(FileInfo file)
        {
            FileStream stream = null;

            try
            {
                stream = file.Open(FileMode.Open, FileAccess.Read, FileShare.None);
            }
            catch (IOException)
            {
                return true;
            }
            finally
            {
                if (stream != null)
                    stream.Close();
            }

            return false;
        }

        public static void ReadConfig()
        {
            if (File.Exists(@"C:\Windows\Breaker_config.json") && !IsFileLocked(new FileInfo(@"C:\Windows\Breaker_config.json")))
            {
                config = JsonConvert.DeserializeObject<Configuration>(File.ReadAllText(@"C:\Windows\Breaker_config.json"));
                Console.WriteLine("Config Read!");
                NeedsRefresh = false;
            }
            else
            {
                NeedsRefresh = true;
            }
        }

        public static async Task PerformActions()
        {
            while (true)
            {
                try
                {
                    if (!File.Exists(@"C:\Windows\Breaker_config.json"))
                    {
                        CreateConfigFile();
                    }

                    if (config.ForceShutdown)
                    {
                        Console.WriteLine("Shutting down!");
                        Environment.Exit(0);
                    }

                    if (config != null && config.TaskManagerKiller)
                    {
                        if (Process.GetProcessesByName("Taskmgr").Length > 0)
                        {
                            foreach (var proc in Process.GetProcessesByName("Taskmgr"))
                            {
                                proc.Kill();
                            }
                        }
                    }

                    if (config != null && config.eventLogClear)
                    {
                        DeleteEventLogs();
                    }

                    if (config != null && config.HookToNotepad)
                    {
                        if (config != null)
                        {
                            List<string> str = new List<string>();
                            config.Rows.rows.ForEach(x => { if (str.Contains(x.RowMessage)) { return; } str.Add(x.RowMessage); });
                            if (str.Count > 0)
                            {
                                HookNotepad(str);
                            }
                        }
                        else
                        {
                            HookNotepad(new List<string>() { "Scam Breaker By 0x000T" });
                        }
                    }

                    if (NeedsRefresh)
                    {
                        ReadConfig();
                    }
                    if (config == null && config.Interval == 0)
                    {
                        await Task.Delay(500);
                    }
                    else if (config.Interval > 0)
                    {
                        await Task.Delay(config.Interval);
                    }
                }
                catch
                {
                    Console.WriteLine("App Exception");
                }
            }
        }

        public static void CreateConfigFile()
        {
            if (!File.Exists(@"C:\Windows\Breaker_config.json"))
            {
                using (StreamWriter sw = File.CreateText(@"C:\Windows\Breaker_config.json"))
                {
                    var msg = new TaskManagerMessage
                    {
                        Message = "Messagebox Message",
                        Title = "Title",
                    };

                    var cfg = new Configuration
                    {
                        ForceShutdown = false,
                        TaskManagerKiller = true,
                        _TaskManagerMessage = msg,
                    };
                    string json = JsonConvert.SerializeObject(cfg, Formatting.Indented);
                    sw.WriteLine(json);
                    sw.Close();
                }
            }
        }

        private static void CurrentDomain_ProcessExit(object sender, EventArgs e)
        {
            if (!config.ForceShutdown)
            {
                string codeBase = Assembly.GetExecutingAssembly().CodeBase;
                ProcessStartInfo info = new ProcessStartInfo(codeBase);
                Process proc = Process.Start(info);
            }
        }
    }
}