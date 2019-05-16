using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using CSLib.UltraKit.Define;

namespace UltraKitTestbed
{
    static class Program
    {

        private static FrameworkModule frameworkModule;

        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            // Initialize framework 
            frameworkModule = FrameworkModule.Instance;
            frameworkModule.FrameworkForm = new UltraKitTestbed();
            frameworkModule.Initialize();

            frameworkModule.FrameworkForm.Visible = true;
            frameworkModule.FrameworkForm.BringToFront();
            frameworkModule.FrameworkForm.Activate();

            // Process command line
            //Script.ArkLua.Instance.ProcessCommadnLine(args);

            Application.Idle += new EventHandler(OnApplicationIdle);
            Application.Run(frameworkModule.FrameworkForm);

            frameworkModule.UnInitialize();

        }



        public static void OnApplicationIdle(object s, EventArgs e)
        {
            while (AppStillIdle)
            {
                frameworkModule.Update();
            }
        }

        public static bool AppStillIdle
        {
            get
            {
                Message msg;
                return !PeekMessage(out msg, IntPtr.Zero, 0, 0, 0);
            }
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct Message
        {
            public IntPtr hWnd;
            public uint msg;
            public IntPtr wParam;
            public IntPtr lParam;
            public uint time;
            public System.Drawing.Point p;
        }

        [System.Security.SuppressUnmanagedCodeSecurity]
        [DllImport("User32.dll")]
        public static extern bool PeekMessage(out Message msg, IntPtr hWnd, uint messageFilterMin, uint messageFilterMax, uint flags);
    }
}
