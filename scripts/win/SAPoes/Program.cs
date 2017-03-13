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

namespace SAPoes
{
    class WatchDialog
    {
        static Int32 checkinterval=2;
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

            while (true)
            {
                Thread.Sleep(WatchDialog.checkinterval * 1000);
                Console.WriteLine("*** Started ***");
                try
                {
                    // Here, check the current window title.
                }
                catch (Exception e)
                {
                    Console.WriteLine(e.Message);
                }
            }
        }
    }
}
