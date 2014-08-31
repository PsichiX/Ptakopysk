using System;
using System.Collections.Generic;
using System.IO;
using System.Text.RegularExpressions;
using Microsoft.Win32;

namespace ZasuvkaPtakopyska
{
    public class Utils
    {
        #region Public Nested Classes.

        public class ReplaceString
        {
            static readonly Dictionary<string, string> m_replaceDict = new Dictionary<string, string>();

            const string ms_regexEscapes = @"[\a\b\f\n\r\t\v\\""]";

            public static string StringLiteral(string i_string)
            {
                return Regex.Replace(i_string, ms_regexEscapes, Match);
            }

            public static string CharLiteral(char c)
            {
                return c == '\'' ? @"'\''" : string.Format("'{0}'", c);
            }

            private static string Match(Match m)
            {
                string match = m.ToString();
                if (m_replaceDict.ContainsKey(match))
                    return m_replaceDict[match];
                throw new NotSupportedException();
            }

            static ReplaceString()
            {
                m_replaceDict.Add("\a", @"\a");
                m_replaceDict.Add("\b", @"\b");
                m_replaceDict.Add("\f", @"\f");
                m_replaceDict.Add("\n", @"\n");
                m_replaceDict.Add("\r", @"\r");
                m_replaceDict.Add("\t", @"\t");
                m_replaceDict.Add("\v", @"\v");
                m_replaceDict.Add("\\", @"\\");
                m_replaceDict.Add("\0", @"\0");
                m_replaceDict.Add("\"", "\\\"");
            }
        }
        
        #endregion



        #region Public Static Functionality.

        public static string GetRelativePath(string path, string relativeTo)
        {
            Uri from = new Uri(path);
            Uri to = new Uri(relativeTo);
            Uri result = to.MakeRelativeUri(from);
            return Uri.UnescapeDataString(result.ToString());
        }

        public static string GetCodeBlocksInstallationPath()
        {
            string path = null;

            string registryKey = @"SOFTWARE\CodeBlocks";
            RegistryKey key = Registry.CurrentUser.OpenSubKey(registryKey);
            if (key != null)
            {
                path = key.GetValue("Path") as string;
                key.Close();
            }

            registryKey = @"SOFTWARE\Wow6432Node\CodeBlocks";
            key = Registry.CurrentUser.OpenSubKey(registryKey);
            if (key != null)
            {
                path = key.GetValue("Path") as string;
                key.Close();
            }

            return path;
        }

        public static string ConvertWindowsToUnixPath(string path)
        {
            bool isRooted = Path.IsPathRooted(path);
            string root = Path.GetPathRoot(path);
            string rest = isRooted ? path.Remove(0, root.Length) : path;
            string unix = "";
            if (isRooted)
            {
                root = root.Replace(":", "");
                root = root.Replace("\\", "");
                unix += "/" + root + "/";
            }
            unix += rest.Replace("\\", "/");
            return unix;
        }

        #endregion
    }
}
