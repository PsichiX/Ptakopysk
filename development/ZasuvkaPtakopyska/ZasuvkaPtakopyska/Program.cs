using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;
using MetroFramework;

namespace ZasuvkaPtakopyska
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            AppDomain.CurrentDomain.UnhandledException += new UnhandledExceptionEventHandler(CurrentDomain_UnhandledException);
            try
            {
                Application.EnableVisualStyles();
                Application.SetCompatibleTextRenderingDefault(false);
                Application.Run(new MainForm());
            }
            catch (Exception ex)
            {
                CurrentDomain_UnhandledException(null, new UnhandledExceptionEventArgs(ex, true));
            }
        }

        static void CurrentDomain_UnhandledException(object sender, UnhandledExceptionEventArgs e)
        {
            try
            {
                Exception ex = e.ExceptionObject as Exception;
                if (ex != null)
                    MetroMessageBox.Show(null, ex.Message + "\n" + ex.StackTrace, "Unhandled Exception", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            finally
            {
                if (e.IsTerminating)
                    Application.Exit();
            }
        }
    }
}
