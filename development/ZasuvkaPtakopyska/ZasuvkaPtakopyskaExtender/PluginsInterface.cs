using System;
using System.IO;
using System.Runtime.InteropServices;

namespace ZasuvkaPtakopyskaExtender
{
    public class PluginsInterface
    {
        #region Private Const Data.

        private const string DLL = "PluginsInterface.dll";

        #endregion



        #region Public Nested Structures.

        public class Result<T>
        {
            public string error = null;
            public T result = default(T);
        }

        #endregion



        #region Public Static Functionality.

        public static bool Load(string path)
        {
            if (string.IsNullOrEmpty(path))
                return false;
            if (!File.Exists(path))
            {
                Console.WriteLine("Plugin library does not exists: " + path);
                return false;
            }
            try
            {
                var result = _PluginLoad(path);
                ErrorsToConsole();
                return result;
            }
            catch (Exception ex)
            {
                Console.WriteLine(">>> " + ex.GetType().Name + ": " + ex.Message);
                return false;
            }
        }

        public static bool Unload(string path)
        {
            if (string.IsNullOrEmpty(path))
                return false;
            try
            {
                var result = _PluginUnload(path);
                ErrorsToConsole();
                return result;
            }
            catch (Exception ex)
            {
                Console.WriteLine(">>> " + ex.GetType().Name + ": " + ex.Message);
                return false;
            }
        }

        public static void UnloadAll()
        {
            try
            {
                _PluginUnloadAll();
                ErrorsToConsole();
            }
            catch (Exception ex)
            {
                Console.WriteLine(">>> " + ex.GetType().Name + ": " + ex.Message);
            }
        }

        public static string[] ListAll()
        {
            try
            {
                var result = Marshal.PtrToStringAnsi(_PluginListAll());
                ErrorsToConsole();
                return string.IsNullOrEmpty(result) ? null : result.Split(new string[] { ";" }, System.StringSplitOptions.RemoveEmptyEntries);
            }
            catch (Exception ex)
            {
                Console.WriteLine(">>> " + ex.GetType().Name + ": " + ex.Message);
                return null;
            }
        }

        public static bool SetCurrent(string path)
        {
            if (string.IsNullOrEmpty(path))
                return false;
            try
            {
                var result = _PluginSetCurrent(path);
                ErrorsToConsole();
                return result;
            }
            catch (Exception ex)
            {
                Console.WriteLine(">>> " + ex.GetType().Name + ": " + ex.Message);
                return false;
            }
        }

        public static string GetCurrent()
        {
            try
            {
                var result = Marshal.PtrToStringAnsi(_PluginGetCurrent());
                ErrorsToConsole();
                return result;
            }
            catch (Exception ex)
            {
                Console.WriteLine(">>> " + ex.GetType().Name + ": " + ex.Message);
                return null;
            }
        }

        public static string Query(string query)
        {
            if (string.IsNullOrEmpty(query))
                return null;
            try
            {
                var result = Marshal.PtrToStringAnsi(_PluginQuery(query));
                ErrorsToConsole();
                return result;
            }
            catch (Exception ex)
            {
                Console.WriteLine(">>> " + ex.GetType().Name + ": " + ex.Message);
                return null;
            }
        }

        public static Result<TR> QueryFunction<TR>(string funcName, params object[] args)
        {
            try
            {
                string a = Newtonsoft.Json.JsonConvert.SerializeObject(args, Newtonsoft.Json.Formatting.None);
                if (!string.IsNullOrEmpty(a))
                {
                    string r = Query("{ \"" + funcName + "\": " + a + " }");
                    ErrorsToConsole();
                    return Newtonsoft.Json.JsonConvert.DeserializeObject<Result<TR>>(r);
                }
                return new Result<TR>();
            }
            catch (Exception ex)
            {
                Console.WriteLine(">>> " + ex.GetType().Name + ": " + ex.Message);
                return new Result<TR>();
            }
        }

        public static void ErrorsToConsole()
        {
            try
            {
                string text = Marshal.PtrToStringAnsi(_PluginErrors());
                Console.WriteLine(text);
            }
            catch (Exception ex)
            {
                Console.WriteLine(">>> " + ex.GetType().Name + ": " + ex.Message);
            }
        }

        #endregion



        #region Private Static Extern Functionality.

        [DllImport(DLL, CallingConvention = CallingConvention.StdCall)]
        [return: MarshalAs(UnmanagedType.I1)]
        private static extern bool _PluginLoad(
            [MarshalAs(UnmanagedType.LPStr)]
            string path
            );

        [DllImport(DLL, CallingConvention = CallingConvention.StdCall)]
        [return: MarshalAs(UnmanagedType.I1)]
        private static extern bool _PluginUnload(
            [MarshalAs(UnmanagedType.LPStr)]
            string path
            );

        [DllImport(DLL, CallingConvention = CallingConvention.StdCall)]
        [return: MarshalAs(UnmanagedType.I1)]
        private static extern void _PluginUnloadAll();

        [DllImport(DLL, CallingConvention = CallingConvention.StdCall)]
        private static extern IntPtr _PluginListAll();

        [DllImport(DLL, CallingConvention = CallingConvention.StdCall)]
        [return: MarshalAs(UnmanagedType.I1)]
        private static extern bool _PluginSetCurrent(
            [MarshalAs(UnmanagedType.LPStr)]
            string path
            );

        [DllImport(DLL, CallingConvention = CallingConvention.StdCall)]
        private static extern IntPtr _PluginGetCurrent();

        [DllImport(DLL, CallingConvention = CallingConvention.StdCall)]
        private static extern IntPtr _PluginQuery(
            [MarshalAs(UnmanagedType.LPStr)]
            string query
            );

        [DllImport(DLL, CallingConvention = CallingConvention.StdCall)]
        private static extern IntPtr _PluginErrors();

        #endregion
    }
}
