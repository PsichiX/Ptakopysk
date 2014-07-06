using System.IO;
using Microsoft.Win32;
using System;

namespace ZasuvkaPtakopyska
{
    public class Utils
    {
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
