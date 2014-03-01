using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace TimusTinyBasicPlugin
{
    /// <summary>
    /// Class that contains constant static values.
    /// </summary>
    public static class Constants
    {
        /// <summary>
        /// Plugin name.
        /// </summary>
        public const String NAME = "Timus Tiny BASIC";

        /// <summary>
        /// New file content template.
        /// </summary>
        public const String FILE_TEMPLATE = "REM Simple \"Hello World\" example.\r\n10 PRINT \"Hello world!\"\r\n";
    }
}
