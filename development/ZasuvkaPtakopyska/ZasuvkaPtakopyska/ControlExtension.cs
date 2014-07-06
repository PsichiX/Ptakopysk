﻿using System;
using System.Windows.Forms;

public static class ControlExtensions
{
    /// <summary>
    /// Executes the Action asynchronously on the UI thread, does not block execution on the calling thread.
    /// </summary>
    /// <param name="control"></param>
    /// <param name="code"></param>
    public static void doOnUIThread(this Control @this, Action code)
    {
        if (@this.InvokeRequired)
        {
            @this.BeginInvoke(code);
        }
        else
        {
            code.Invoke();
        }
    }
}
