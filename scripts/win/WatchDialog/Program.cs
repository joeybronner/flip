using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Windows.Automation;
using System.Windows.Forms;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;




namespace UIAutomation
{
    class WatchDialog
    {
        static List<String[]> parameters = new List<String[]>();
        static Int32 checkinterval=30, displaytime=30;
        static String screenshotPath="";

        static void Main(String[] args)
        {
            // check if an instance is already running
            /*
            if (Process.GetProcessesByName("WatchDialog").Length > 1)
            {
                Console.WriteLine("Only one instance can run at a time");
                Thread.Sleep(3000);
                return;
            }
            */

            // parse parameters
            String paramType = "-watchlist";
            foreach (String arg in args)
            {
                // parameter type
                if (arg.StartsWith("-"))
                {
                    if (arg == "-help") { showUsage(); return; }
                    paramType = arg;
                    continue;
                }

                // parameter arguments
                switch(paramType)
                {
                    case "-watchlist":
                        String[] parameter = arg.Split('|');
                        if (parameter.Length != 3)
                        {
                            showUsage();
                            return;
                        }
                        parameters.Add(parameter);
                        break;
                    case "-checkinterval":
                        checkinterval = Int32.Parse(arg);
                        break;
                    case "-displaytime":
                        displaytime = Int32.Parse(arg);
                        break;
                    case "-screenshot":
                        if (!new DirectoryInfo(arg).Exists)
                        {
                            Console.WriteLine("'{0}' is not a valid directory", arg);
                            showUsage();
                            return;
                        }
                        screenshotPath = arg;
                        break;
                }
            }

            // start
            start();
        }

        public static void showUsage()
        {
            Console.WriteLine("*** Usage ***");
            Console.WriteLine("WatchDialog [-watchlist] <list> [-checkinterval <x>] [-displaytime <y>] [-screenshot <path>]");
            Console.WriteLine("");
            Console.WriteLine("<list>:");
            Console.WriteLine("  \"[exe1]|[dialog1]|name1\" \"[exe2]|[dialog2]|name2\"...");
            Console.WriteLine("  - exe: name of the executable owning the dialog box to watch (optional)");
            Console.WriteLine("  - dialog: name of the dialog box to watch (optional)");
            Console.WriteLine("  - name: name of button to click on (mandatory)");
            Console.WriteLine("<x>:");
            Console.WriteLine("  delay in seconds between two checks");
            Console.WriteLine("  (default is 30s)");
            Console.WriteLine("<y>:");
            Console.WriteLine("  minimal time in seconds a dialog must be displayed before being clicked");
            Console.WriteLine("<path>:");
            Console.WriteLine("  path to a directory in which screenshots are saved prior to clicking");
        }

        public static void start()
        {
            Console.WriteLine("*** Watch List ***");
            foreach (String[] parameter in WatchDialog.parameters) Console.WriteLine(" - {0}|{1}|{2}", parameter[0], parameter[1], parameter[2]);
            Console.WriteLine("*** Started ***");

            Dictionary<int, DateTime> watchList = new Dictionary<int, DateTime>();
            while (true)
            {
                // look for new elements
                foreach (String[] parameter in WatchDialog.parameters) // for each button to watch
                {
                    AutomationElement element = watchDialog(parameter[0], parameter[1], parameter[2]);

                    if (element == null) continue;

                    Console.WriteLine("{0}: [{1}] found", DateTime.Now, element.Current.Name);
                    if (!watchList.ContainsKey(element.Current.NativeWindowHandle))
                    {
                        watchList.Add(element.Current.NativeWindowHandle, DateTime.Now);
                    }
                }

                // check the watch list
                Dictionary<int, DateTime> newWatchList = new Dictionary<int, DateTime>();
                foreach (int key in watchList.Keys)
                {
                    AutomationElement element;

                    try
                    {
                        element = AutomationElement.FromHandle((IntPtr)key);
                    }
                    catch (ElementNotAvailableException)
                    {
                        continue;
                    }

                    String elementName = element.Current.Name;
                    TimeSpan elapsedTime = DateTime.Now - watchList[key];
                    if (elapsedTime.TotalSeconds >= WatchDialog.displaytime) // click on button if displayed more than displaytime s
                    {
                        Console.WriteLine("{0}: [{1}] displayed for more than {2}s", DateTime.Now, elementName, displaytime);

                        // screenshot
                        try
                        {
                            if (screenshotPath != "")
                            {
                                String filePath = screenshotPath + @"\" + DateTime.Now.Ticks + ".png";
                                element.SetFocus();
                                takeScreenshot(filePath);
                                Console.WriteLine("{0}: screenshot saved to '" + filePath + "'", DateTime.Now);
                            }
                        }
                        catch (Exception e)
                        {
                            Console.WriteLine("{0}: failed to take screenshot ({1})", DateTime.Now, e.Message);
                        }

                        // click
                        try
                        {
                            click(element);
                            Console.WriteLine("{0}: [{1}] clicked", DateTime.Now, elementName);
                        }
                        catch (Exception e)
                        {
                            Console.WriteLine("{0}: [{1}] failed to click ({2})", DateTime.Now, elementName, e.Message);
                        }
                    }
                    else // continue to watch
                    {
                        newWatchList.Add(key, watchList[key]);
                    }
                }
                watchList = newWatchList;

                // wait before next check
                Thread.Sleep(WatchDialog.checkinterval * 1000);
            }
        }

        public static AutomationElement watchDialog(String processName, String dialogName, String buttonName)
        {
            bool checkProcessName = (processName != null) && (processName.Length > 0);
            bool checkDialogName = (dialogName != null) && (dialogName.Length > 0);

            AutomationElement element = null;
            try
            {
                // check process name beforehand
                if (checkProcessName && (Process.GetProcessesByName(processName).Length == 0)) return null;

                // check dialog box name
                AutomationElement dialogbox = null;
                if (checkDialogName)
                {
                    Condition windowCondition = new PropertyCondition(AutomationElement.ControlTypeProperty, ControlType.Window);
                    Condition titleCondition = new PropertyCondition(AutomationElement.NameProperty, dialogName);
                    dialogbox = AutomationElement.RootElement.FindFirst(TreeScope.Descendants, new AndCondition(titleCondition, windowCondition));
                }
                if (checkDialogName && (dialogbox == null)) return null;

                // check button name
                Condition buttonCondition = new PropertyCondition(AutomationElement.ControlTypeProperty, ControlType.Button);
                Condition labelCondition = new PropertyCondition(AutomationElement.NameProperty, buttonName);
                AutomationElement container = (dialogbox == null) ? AutomationElement.RootElement : dialogbox;
                AutomationElement button = container.FindFirst(TreeScope.Descendants, new AndCondition(labelCondition, buttonCondition));
                if (button == null) return null;

                // check process name
                Process process = Process.GetProcessById(button.Current.ProcessId);
                if (checkProcessName && (process.ProcessName != processName)) return null;

                element = button;
            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);
            }

            return element;
        }

        public static void click(AutomationElement element)
        {
            InvokePattern invoke = (InvokePattern)element.GetCurrentPattern(InvokePattern.Pattern);
            invoke.Invoke();
        }

        public static void takeScreenshot(String filePath)
        {
            Bitmap bmp = new Bitmap(Screen.PrimaryScreen.Bounds.Width, Screen.PrimaryScreen.Bounds.Height);
            Graphics screenshot = Graphics.FromImage(bmp);
            screenshot.CopyFromScreen(Screen.PrimaryScreen.Bounds.X, Screen.PrimaryScreen.Bounds.Y, 0, 0, Screen.PrimaryScreen.Bounds.Size, CopyPixelOperation.SourceCopy);
            bmp.Save(filePath, ImageFormat.Png);
        }
    }
}
