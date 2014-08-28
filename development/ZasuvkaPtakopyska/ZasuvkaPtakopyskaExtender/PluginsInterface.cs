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
            try
            {
                return _PluginLoad(path);
            }
            catch { return false; }
        }

        public static bool Unload(string path)
        {
            try
            {
                return _PluginUnload(path);
            }
            catch { return false; }
        }

        public static void UnloadAll()
        {
            try
            {
                _PluginUnloadAll();
            }
            catch { }
        }

        public static string[] ListAll()
        {
            try
            {
                string result = _PluginListAll();
                return string.IsNullOrEmpty(result) ? null : result.Split(new string[] { ";" }, System.StringSplitOptions.RemoveEmptyEntries);
            }
            catch { return null; }
        }

        public static bool SetCurrent(string path)
        {
            if (string.IsNullOrEmpty(path))
                return false;
            try
            {
                return _PluginSetCurrent(path);
            }
            catch { return false; }
        }

        public static string GetCurrent()
        {
            try
            {
                return _PluginGetCurrent();
            }
            catch { return null; }
        }

        public static string Query(string query)
        {
            try
            {
                return _PluginQuery(query);
            }
            catch { return null; }
        }

        public static Result<TR> QueryFunction<TR>(string funcName, params object[] args)
        {
            try
            {
                string a = Newtonsoft.Json.JsonConvert.SerializeObject(args, Newtonsoft.Json.Formatting.None);
                if (!string.IsNullOrEmpty(a))
                {
                    string r = Query("{ \"" + funcName + "\": " + a + " }");
                    return Newtonsoft.Json.JsonConvert.DeserializeObject<Result<TR>>(r);
                }
                return new Result<TR>();
            }
            catch { return new Result<TR>(); }
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
        [return: MarshalAs(UnmanagedType.LPStr)]
        private static extern string _PluginListAll();

        [DllImport(DLL, CallingConvention = CallingConvention.StdCall)]
        [return: MarshalAs(UnmanagedType.I1)]
        private static extern bool _PluginSetCurrent(
            [MarshalAs(UnmanagedType.LPStr)]
            string path
            );

        [DllImport(DLL, CallingConvention = CallingConvention.StdCall)]
        [return: MarshalAs(UnmanagedType.LPStr)]
        private static extern string _PluginGetCurrent();

        [DllImport(DLL, CallingConvention = CallingConvention.StdCall)]
        [return: MarshalAs(UnmanagedType.LPStr)]
        private static extern string _PluginQuery(
            [MarshalAs(UnmanagedType.LPStr)]
            string query
            );

        #endregion
    }
}
