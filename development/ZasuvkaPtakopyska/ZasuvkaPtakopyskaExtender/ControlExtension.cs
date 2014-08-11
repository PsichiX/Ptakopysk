using System;
using System.Windows.Forms;
using System.Drawing;
using ZasuvkaPtakopyskaExtender;

public static class ControlExtensions
{
    public static void DoOnUiThread(this Control @this, Action code)
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

    public static bool CalculateContentsRectangle(this Control @this, out Rectangle result, Func<Control, Rectangle, Rectangle> forEach = null)
    {
        Rectangle rect;
        Rectangle union = new Rectangle();
        bool first = true;
        foreach (Control c in @this.Controls)
        {
            if (forEach == null)
                rect = new Rectangle(c.Location, c.Size);
            else
                rect = forEach(c, new Rectangle(c.Location, c.Size));
            if (!first)
                union = Rectangle.Union(rect, union);
            else
            {
                union = rect;
                first = false;
            }
        }
        result = union;
        return !first;
    }
}
