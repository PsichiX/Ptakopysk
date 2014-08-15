using ZasuvkaPtakopyskaExtender;
using System;
using System.Globalization;

[assembly:ZasuvkaPtakopyskaExtender]

public static class Settings
{
    public static readonly string DEFAULT_STRING_FORMAT = null;
    
    private static NumberStyles s_numberStyle = NumberStyles.Any;
    private static IFormatProvider s_formatProvider = CultureInfo.InvariantCulture;

    public static NumberStyles DefaultNumberStyle { get { return s_numberStyle; } set { s_numberStyle = value; } }
    public static IFormatProvider DefaultFormatProvider { get { return s_formatProvider; } set { s_formatProvider = value; } }
}
