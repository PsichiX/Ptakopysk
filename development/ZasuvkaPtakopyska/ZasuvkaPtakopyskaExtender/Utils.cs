using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Reflection;
using System.Text.RegularExpressions;
using Microsoft.Win32;

namespace ZasuvkaPtakopyskaExtender
{
    public static class Utils
    {
        #region Public Nested Classes.

        public class EscapedString
        {
            static readonly Dictionary<string, string> m_replaceDict = new Dictionary<string, string>();

            const string ms_regexEscapes = @"[\a\b\f\n\r\t\v\\""]";

            public static string Escape(string s)
            {
                return Regex.Replace(s, ms_regexEscapes, MatchEscape);
            }

            public static string Unescape(string s)
            {
                return Regex.Unescape(s);
            }

            private static string MatchEscape(Match m)
            {
                string match = m.ToString();
                if (m_replaceDict.ContainsKey(match))
                    return m_replaceDict[match];
                throw new NotSupportedException();
            }

            static EscapedString()
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

        public static bool TryParse<T>(string v, out T r, T d, NumberStyles ns, IFormatProvider fp) where T : IFormattable
        {
            MethodInfo method = null;
            try { method = typeof(T).GetMethod("Parse", new Type[] { typeof(string), typeof(NumberStyles), typeof(IFormatProvider) }); }
            catch
            {
                r = d;
                return false;
            }
            if (method == null)
            {
                r = d;
                return false;
            }
            try
            {
                r = (T)method.Invoke(null, new object[] { v, ns, fp });
                return true;
            }
            catch
            {
                r = d;
                return false;
            }
        }

        public static List<object> GenerateAdditionalArguments(Type type, Newtonsoft.Json.Linq.JToken data)
        {
            if (type == null || data == null)
                return null;

            MethodInfo method;
            try { method = type.GetMethod("GenerateAdditionalArguments", new Type[] { typeof(Newtonsoft.Json.Linq.JToken) }); }
            catch { return null; }
            if (method == null || !method.IsStatic)
                return null;

            try { return (List<object>)method.Invoke(null, new object[] { data }); }
            catch { return null; }
        }

        #endregion
    }
}
