using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace ZasuvkaPtakopyskaExtender
{
    public class PtakopyskInterface
    {
        #region Public Enumerations.

        public enum AssetType
        {
            Texture = 0,
            Shader = 1,
            Sound = 2,
            Music = 3,
            Font = 4
        }

        #endregion



        #region Private Const Data.

        private const string DLL = "PtakopyskInterface.dll";

        #endregion



        #region Private Static Data.

        private static PtakopyskInterface s_instance;

        #endregion



        #region Public Static Properties.

        public static PtakopyskInterface Instance { get { if (s_instance == null) s_instance = new PtakopyskInterface(); return s_instance; } }

        #endregion



        #region Public Functionality.

        public bool Initialize(int windowHandle, string defaultTexturePath, string defaultVertexShaderPath, string defaultFragmentShaderPath, string defaultFontPath)
        {
            try
            {
                bool status = _Initialize(windowHandle, defaultTexturePath, defaultVertexShaderPath, defaultFragmentShaderPath, defaultFontPath);
                Console.Write(_PopErrors());
                return status;
            }
            catch (Exception ex) { LogException(ex); return false; }
        }

        public void Release()
        {
            try { _Release(); }
            catch (Exception ex) { LogException(ex); }
        }

        public void SetAssetsFileSystemRoot(string path)
        {
            try { _SetAssetsFileSystemRoot(path); }
            catch (Exception ex) { LogException(ex); }
        }

        public bool ProcessEvents()
        {
            try
            {
                bool status = _ProcessEvents();
                Console.Write(_PopErrors());
                return status;
            }
            catch (Exception ex) { LogException(ex); return false; }
        }

        public bool ProcessPhysics(float deltaTime, int velocityIterations, int positionIterations)
        {
            try
            {
                bool status = _ProcessPhysics(deltaTime, velocityIterations, positionIterations);
                Console.Write(_PopErrors());
                return status;
            }
            catch (Exception ex) { LogException(ex); return false; }
        }

        public bool ProcessUpdate(float deltaTime, bool sortInstances)
        {
            try
            {
                bool status = _ProcessUpdate(deltaTime, sortInstances);
                Console.Write(_PopErrors());
                return status;
            }
            catch (Exception ex) { LogException(ex); return false; }
        }

        public bool ProcessRender(bool liveOrEditor)
        {
            try
            {
                bool status = _ProcessRender(liveOrEditor);
                Console.Write(_PopErrors());
                return status;
            }
            catch (Exception ex) { LogException(ex); return false; }
        }

        public bool SetVerticalSyncEnabled(bool enabled)
        {
            try
            {
                bool status = _SetVerticalSyncEnabled(enabled);
                Console.Write(_PopErrors());
                return status;
            }
            catch (Exception ex) { LogException(ex); return false; }
        }

        public float GetGridSizeX()
        {
            try { return _GetGridSizeX(); }
            catch (Exception ex) { LogException(ex); }
            return 0.0f;
        }

        public float GetGridSizeY()
        {
            try { return _GetGridSizeY(); }
            catch (Exception ex) { LogException(ex); }
            return 0.0f;
        }

        public void SetGridSize(float x, float y)
        {
            try { _SetGridSize(x, y); }
            catch (Exception ex) { LogException(ex); }
        }

        public float GetSceneViewSizeX()
        {
            try { return _GetSceneViewSizeX(); }
            catch (Exception ex) { LogException(ex); }
            return 0.0f;
        }

        public float GetSceneViewSizeY()
        {
            try { return _GetSceneViewSizeY(); }
            catch (Exception ex) { LogException(ex); }
            return 0.0f;
        }

        public void SetSceneViewSize(float x, float y)
        {
            try { _SetSceneViewSize(x, y); }
            catch (Exception ex) { LogException(ex); }
        }

        public float GetSceneViewCenterX()
        {
            try { return _GetSceneViewCenterX(); }
            catch (Exception ex) { LogException(ex); }
            return 0.0f;
        }

        public float GetSceneViewCenterY()
        {
            try { return _GetSceneViewCenterY(); }
            catch (Exception ex) { LogException(ex); }
            return 0.0f;
        }

        public void SetSceneViewCenter(float x, float y)
        {
            try { _SetSceneViewCenter(x, y); }
            catch (Exception ex) { LogException(ex); }
        }

        public float GetSceneViewZoom()
        {
            try { return _GetSceneViewZoom(); }
            catch (Exception ex) { LogException(ex); }
            return 0.0f;
        }

        public void SetSceneViewZoom(float zoom)
        {
            try { _SetSceneViewZoom(zoom); }
            catch (Exception ex) { LogException(ex); }
        }

        public bool ClearScene()
        {
            try
            {
                bool status = _ClearScene();
                Console.Write(_PopErrors());
                return status;
            }
            catch (Exception ex) { LogException(ex); return false; }
        }

        public bool ApplyJsonToScene(string json)
        {
            try
            {
                bool status = _ApplyJsonToScene(json);
                Console.Write(_PopErrors());
                return status;
            }
            catch (Exception ex) { LogException(ex); return false; }
        }

        public string ConvertSceneToJson()
        {
            try
            {
                string result = _ConvertSceneToJson();
                Console.Write(_PopErrors());
                return result;
            }
            catch (Exception ex) { LogException(ex); return null; }
        }

        public int CreateGameObject(bool isPrefab, int parent, string prefabSource, string id)
        {
            try
            {
                int handle = _CreateGameObject(isPrefab, parent, prefabSource, id);
                Console.Write(_PopErrors());
                return handle;
            }
            catch (Exception ex) { LogException(ex); return 0; }
        }

        public bool DestroyGameObject(int handle, bool isPrefab)
        {
            try
            {
                bool status = _DestroyGameObject(handle, isPrefab);
                Console.Write(_PopErrors());
                return status;
            }
            catch (Exception ex) { LogException(ex); return false; }
        }

        public bool ClearGameObject(int handle, bool isPrefab)
        {
            try
            {
                bool status = _ClearGameObject(handle, isPrefab);
                Console.Write(_PopErrors());
                return status;
            }
            catch (Exception ex) { LogException(ex); return false; }
        }

        public bool ApplyJsonToGameObject(int handle, bool isPrefab, string json)
        {
            try
            {
                bool status = _ApplyJsonToGameObject(handle, isPrefab, json);
                Console.Write(_PopErrors());
                return status;
            }
            catch (Exception ex) { LogException(ex); return false; }
        }

        public string ConvertGameObjectToJson(int handle, bool isPrefab)
        {
            try
            {
                string result = _ConvertGameObjectToJson(handle, isPrefab);
                Console.Write(_PopErrors());
                return result;
            }
            catch (Exception ex) { LogException(ex); return null; }
        }

        public bool StartQueryGameObject(int handle, bool isPrefab)
        {
            try
            {
                bool status = _StartQueryGameObject(handle, isPrefab);
                Console.Write(_PopErrors());
                return status;
            }
            catch (Exception ex) { LogException(ex); return false; }
        }

        public Dictionary<string, string> QueryGameObject(string query)
        {
            try
            {
                bool status = _QueryGameObject(query);
                Console.Write(_PopErrors());
                if (!status)
                    return null;
                if (_QueriedGameObjectResultsCount() == 0)
                    return null;
                Dictionary<string, string> result = new Dictionary<string, string>();
                do { result[_QueriedGameObjectResultKey()] = _QueriedGameObjectResultValue(); }
                while (!_QueriedGameObjectResultNext());
                return result;
            }
            catch (Exception ex) { LogException(ex); return null; }
        }

        public Dictionary<string, string> QueryGameObject(int handle, bool isPrefab, string query)
        {
            StartQueryGameObject(handle, isPrefab);
            Dictionary<string, string> result = QueryGameObject(query);
            EndQueryGameObject();
            return result;
        }

        public int QueriedGameObjectHandle()
        {
            try
            {
                int result = _QueriedGameObjectHandle();
                Console.Write(_PopErrors());
                return result;
            }
            catch (Exception ex) { LogException(ex); return 0; }
        }

        public void EndQueryGameObject()
        {
            try { _EndQueryGameObject(); }
            catch (Exception ex) { LogException(ex); }
        }

        public bool StartIterateGameObjects(bool isPrefab)
        {
            try
            {
                bool status = _StartIterateGameObjects(isPrefab);
                Console.Write(_PopErrors());
                return status;
            }
            catch (Exception ex) { LogException(ex); return false; }
        }

        public bool CanIterateGameObjectsNext(bool isPrefab)
        {
            try
            {
                bool status = _CanIterateGameObjectsNext(isPrefab);
                Console.Write(_PopErrors());
                return status;
            }
            catch (Exception ex) { LogException(ex); return false; }
        }

        public bool IterateGameObjectsNext(bool isPrefab)
        {
            try
            {
                bool status = _IterateGameObjectsNext(isPrefab);
                Console.Write(_PopErrors());
                return status;
            }
            catch (Exception ex) { LogException(ex); return false; }
        }

        public bool StartQueryIteratedGameObject()
        {
            try
            {
                bool status = _StartQueryIteratedGameObject();
                Console.Write(_PopErrors());
                return status;
            }
            catch (Exception ex) { LogException(ex); return false; }
        }

        public Dictionary<string, string> QueryIteratedGameObject(string query)
        {
            if (StartQueryIteratedGameObject())
            {
                Dictionary<string, string> result = QueryGameObject(query);
                EndQueryGameObject();
                return result;
            }
            return null;
        }

        public bool EndIterateGameObjects()
        {
            try
            {
                bool status = _EndIterateGameObjects();
                Console.Write(_PopErrors());
                return status;
            }
            catch (Exception ex) { LogException(ex); return false; }
        }

        public int FindGameObjectHandleById(string id, bool isPrefab, int parent)
        {
            try
            {
                int result = _FindGameObjectHandleById(id, isPrefab, parent);
                Console.Write(_PopErrors());
                return result;
            }
            catch (Exception ex) { LogException(ex); return 0; }
        }

        public void StartIterateAssets(AssetType type)
        {
            try { _StartIterateAssets((int)type); }
            catch (Exception ex) { LogException(ex); }
        }

        public bool CanIterateAssetsNext(AssetType type)
        {
            try
            {
                bool status = _CanIterateAssetsNext((int)type);
                Console.Write(_PopErrors());
                return status;
            }
            catch (Exception ex) { LogException(ex); return false; }
        }

        public bool IterateAssetsNext(AssetType type)
        {
            try
            {
                bool status = _IterateAssetsNext((int)type);
                Console.Write(_PopErrors());
                return status;
            }
            catch (Exception ex) { LogException(ex); return false; }
        }

        public string GetIteratedAssetId(AssetType type)
        {
            try
            {
                string result = _GetIteratedAssetId((int)type);
                Console.Write(_PopErrors());
                return result;
            }
            catch (Exception ex) { LogException(ex); return null; }
        }

        public void EndIterateAssets(AssetType type)
        {
            try { _EndIterateAssets((int)type); }
            catch (Exception ex) { LogException(ex); }
        }

        public int PluginLoadComponents(string path)
        {
            try
            {
                int result = _PluginLoadComponents(path);
                Console.Write(_PopErrors());
                return result;
            }
            catch (Exception ex) { LogException(ex); return 0; }
        }

        public bool PluginUnloadComponents(int handle)
        {
            try
            {
                bool status = _PluginUnloadComponents(handle);
                Console.Write(_PopErrors());
                return status;
            }
            catch (Exception ex) { LogException(ex); return false; }
        }

        public bool PluginUnloadComponentsByPath(string path)
        {
            try
            {
                bool status = _PluginUnloadComponentsByPath(path);
                Console.Write(_PopErrors());
                return status;
            }
            catch (Exception ex) { LogException(ex); return false; }
        }

        public void StartIterateComponents()
        {
            try { _StartIterateComponents(); }
            catch (Exception ex) { LogException(ex); }
        }

        public bool CanIterateComponentsNext()
        {
            try
            {
                bool status = _CanIterateComponentsNext();
                Console.Write(_PopErrors());
                return status;
            }
            catch (Exception ex) { LogException(ex); return false; }
        }

        public bool IterateComponentsNext()
        {
            try
            {
                bool status = _IterateComponentsNext();
                Console.Write(_PopErrors());
                return status;
            }
            catch (Exception ex) { LogException(ex); return false; }
        }

        public string GetIteratedComponentId()
        {
            try
            {
                string result = _GetIteratedComponentId();
                Console.Write(_PopErrors());
                return result;
            }
            catch (Exception ex) { LogException(ex); return null; }
        }

        public void EndIterateComponents()
        {
            try { _EndIterateComponents(); }
            catch (Exception ex) { LogException(ex); }
        }

        public List<string> GetComponentsIds()
        {
            List<string> result = new List<string>();
            StartIterateComponents();
            while (CanIterateComponentsNext())
            {
                result.Add(GetIteratedComponentId());
                IterateComponentsNext();
            }
            EndIterateComponents();
            return result;
        }

        #endregion



        #region Private Functionality.

        private void LogException(Exception ex)
        {
            string levels = ">";
            while (ex != null)
            {
                Console.WriteLine(string.Format("{0} {1}: {2}", levels, ex.GetType(), ex.Message));
                levels += ">";
                ex = ex.InnerException;
            }
        }

        #endregion



        #region Private Static Extern Functionality.

        [DllImport(DLL, CallingConvention = CallingConvention.StdCall)]
        [return: MarshalAs(UnmanagedType.LPStr)]
        private static extern string _PopErrors();

        [DllImport(DLL, CallingConvention = CallingConvention.StdCall)]
        [return: MarshalAs(UnmanagedType.U1)]
        private static extern bool _Initialize(
            int windowHandle,
            [MarshalAs(UnmanagedType.LPStr)]
            string defaultTexturePath,
            [MarshalAs(UnmanagedType.LPStr)]
            string defaultVertexShaderPath,
            [MarshalAs(UnmanagedType.LPStr)]
            string defaultFragmentShaderPath,
            [MarshalAs(UnmanagedType.LPStr)]
            string defaultFontPath
            );

        [DllImport(DLL, CallingConvention = CallingConvention.StdCall)]
        private static extern void _Release();

        [DllImport(DLL, CallingConvention = CallingConvention.StdCall)]
        private static extern void _SetAssetsFileSystemRoot(
            [MarshalAs(UnmanagedType.LPStr)]
            string path
            );

        [DllImport(DLL, CallingConvention = CallingConvention.StdCall)]
        [return: MarshalAs(UnmanagedType.U1)]
        private static extern bool _ProcessEvents();

        [DllImport(DLL, CallingConvention = CallingConvention.StdCall)]
        [return: MarshalAs(UnmanagedType.U1)]
        private static extern bool _ProcessPhysics(float deltaTime, int velocityIterations, int positionIterations);

        [DllImport(DLL, CallingConvention = CallingConvention.StdCall)]
        [return: MarshalAs(UnmanagedType.U1)]
        private static extern bool _ProcessUpdate(float deltaTime, bool sortInstances);

        [DllImport(DLL, CallingConvention = CallingConvention.StdCall)]
        [return: MarshalAs(UnmanagedType.U1)]
        private static extern bool _ProcessRender(bool liveOrEditor);

        [DllImport(DLL, CallingConvention = CallingConvention.StdCall)]
        [return: MarshalAs(UnmanagedType.U1)]
        private static extern bool _SetVerticalSyncEnabled(bool enabled);

        [DllImport(DLL, CallingConvention = CallingConvention.StdCall)]
        [return: MarshalAs(UnmanagedType.R4)]
        private static extern float _GetGridSizeX();

        [DllImport(DLL, CallingConvention = CallingConvention.StdCall)]
        [return: MarshalAs(UnmanagedType.R4)]
        private static extern float _GetGridSizeY();

        [DllImport(DLL, CallingConvention = CallingConvention.StdCall)]
        private static extern void _SetGridSize(float x, float y);

        [DllImport(DLL, CallingConvention = CallingConvention.StdCall)]
        [return: MarshalAs(UnmanagedType.R4)]
        private static extern float _GetSceneViewSizeX();

        [DllImport(DLL, CallingConvention = CallingConvention.StdCall)]
        [return: MarshalAs(UnmanagedType.R4)]
        private static extern float _GetSceneViewSizeY();

        [DllImport(DLL, CallingConvention = CallingConvention.StdCall)]
        private static extern void _SetSceneViewSize(float x, float y);

        [DllImport(DLL, CallingConvention = CallingConvention.StdCall)]
        [return: MarshalAs(UnmanagedType.R4)]
        private static extern float _GetSceneViewCenterX();

        [DllImport(DLL, CallingConvention = CallingConvention.StdCall)]
        [return: MarshalAs(UnmanagedType.R4)]
        private static extern float _GetSceneViewCenterY();

        [DllImport(DLL, CallingConvention = CallingConvention.StdCall)]
        private static extern void _SetSceneViewCenter(float x, float y);

        [DllImport(DLL, CallingConvention = CallingConvention.StdCall)]
        [return: MarshalAs(UnmanagedType.R4)]
        private static extern float _GetSceneViewZoom();

        [DllImport(DLL, CallingConvention = CallingConvention.StdCall)]
        private static extern void _SetSceneViewZoom(float zoom);

        [DllImport(DLL, CallingConvention = CallingConvention.StdCall)]
        [return: MarshalAs(UnmanagedType.U1)]
        private static extern bool _ClearScene();

        [DllImport(DLL, CallingConvention = CallingConvention.StdCall)]
        [return: MarshalAs(UnmanagedType.U1)]
        private static extern bool _ApplyJsonToScene(
            [MarshalAs(UnmanagedType.LPStr)]
            string json
            );

        [DllImport(DLL, CallingConvention = CallingConvention.StdCall)]
        [return: MarshalAs(UnmanagedType.LPStr)]
        private static extern string _ConvertSceneToJson();

        [DllImport(DLL, CallingConvention = CallingConvention.StdCall)]
        [return: MarshalAs(UnmanagedType.I4)]
        private static extern int _CreateGameObject(
            bool isPrefab,
            int parent,
            [MarshalAs(UnmanagedType.LPStr)]
            string prefabSource,
            [MarshalAs(UnmanagedType.LPStr)]
            string id
            );

        [DllImport(DLL, CallingConvention = CallingConvention.StdCall)]
        [return: MarshalAs(UnmanagedType.U1)]
        private static extern bool _DestroyGameObject(int handle, bool isPrefab);

        [DllImport(DLL, CallingConvention = CallingConvention.StdCall)]
        [return: MarshalAs(UnmanagedType.U1)]
        private static extern bool _ClearGameObject(int handle, bool isPrefab);

        [DllImport(DLL, CallingConvention = CallingConvention.StdCall)]
        [return: MarshalAs(UnmanagedType.U1)]
        private static extern bool _ApplyJsonToGameObject(
            int handle,
            bool isPrefab,
            [MarshalAs(UnmanagedType.LPStr)]
            string json
            );

        [DllImport(DLL, CallingConvention = CallingConvention.StdCall)]
        [return: MarshalAs(UnmanagedType.LPStr)]
        private static extern string _ConvertGameObjectToJson(int handle, bool isPrefab);

        [DllImport(DLL, CallingConvention = CallingConvention.StdCall)]
        [return: MarshalAs(UnmanagedType.U1)]
        private static extern bool _StartQueryGameObject(int handle, bool isPrefab);

        [DllImport(DLL, CallingConvention = CallingConvention.StdCall)]
        [return: MarshalAs(UnmanagedType.U1)]
        private static extern bool _QueryGameObject(
            [MarshalAs(UnmanagedType.LPStr)]
            string query
            );

        [DllImport(DLL, CallingConvention = CallingConvention.StdCall)]
        [return: MarshalAs(UnmanagedType.I4)]
        private static extern int _QueriedGameObjectHandle();

        [DllImport(DLL, CallingConvention = CallingConvention.StdCall)]
        [return: MarshalAs(UnmanagedType.U4)]
        private static extern uint _QueriedGameObjectResultsCount();

        [DllImport(DLL, CallingConvention = CallingConvention.StdCall)]
        [return: MarshalAs(UnmanagedType.U1)]
        private static extern bool _QueriedGameObjectResultNext();

        [DllImport(DLL, CallingConvention = CallingConvention.StdCall)]
        [return: MarshalAs(UnmanagedType.LPStr)]
        private static extern string _QueriedGameObjectResultKey();

        [DllImport(DLL, CallingConvention = CallingConvention.StdCall)]
        [return: MarshalAs(UnmanagedType.LPStr)]
        private static extern string _QueriedGameObjectResultValue();

        [DllImport(DLL, CallingConvention = CallingConvention.StdCall)]
        private static extern void _EndQueryGameObject();

        [DllImport(DLL, CallingConvention = CallingConvention.StdCall)]
        [return: MarshalAs(UnmanagedType.U1)]
        private static extern bool _StartIterateGameObjects(bool isPrefab);

        [DllImport(DLL, CallingConvention = CallingConvention.StdCall)]
        [return: MarshalAs(UnmanagedType.U1)]
        private static extern bool _CanIterateGameObjectsNext(bool isPrefab);

        [DllImport(DLL, CallingConvention = CallingConvention.StdCall)]
        [return: MarshalAs(UnmanagedType.U1)]
        private static extern bool _IterateGameObjectsNext(bool isPrefab);

        [DllImport(DLL, CallingConvention = CallingConvention.StdCall)]
        [return: MarshalAs(UnmanagedType.U1)]
        private static extern bool _StartQueryIteratedGameObject();

        [DllImport(DLL, CallingConvention = CallingConvention.StdCall)]
        [return: MarshalAs(UnmanagedType.U1)]
        private static extern bool _EndIterateGameObjects();

        [DllImport(DLL, CallingConvention = CallingConvention.StdCall)]
        [return: MarshalAs(UnmanagedType.I4)]
        private static extern int _FindGameObjectHandleById(
            [MarshalAs(UnmanagedType.LPStr)]
            string id,
            bool isPrefab,
            int parent
            );

        [DllImport(DLL, CallingConvention = CallingConvention.StdCall)]
        private static extern void _StartIterateAssets(int type);

        [DllImport(DLL, CallingConvention = CallingConvention.StdCall)]
        [return: MarshalAs(UnmanagedType.U1)]
        private static extern bool _CanIterateAssetsNext(int type);

        [DllImport(DLL, CallingConvention = CallingConvention.StdCall)]
        [return: MarshalAs(UnmanagedType.U1)]
        private static extern bool _IterateAssetsNext(int type);

        [DllImport(DLL, CallingConvention = CallingConvention.StdCall)]
        [return: MarshalAs(UnmanagedType.LPStr)]
        private static extern string _GetIteratedAssetId(int type);

        [DllImport(DLL, CallingConvention = CallingConvention.StdCall)]
        private static extern void _EndIterateAssets(int type);

        [DllImport(DLL, CallingConvention = CallingConvention.StdCall)]
        [return: MarshalAs(UnmanagedType.I4)]
        private static extern int _PluginLoadComponents(
            [MarshalAs(UnmanagedType.LPStr)]
            string path
            );

        [DllImport(DLL, CallingConvention = CallingConvention.StdCall)]
        [return: MarshalAs(UnmanagedType.I1)]
        private static extern bool _PluginUnloadComponents(int handle);

        [DllImport(DLL, CallingConvention = CallingConvention.StdCall)]
        [return: MarshalAs(UnmanagedType.I1)]
        private static extern bool _PluginUnloadComponentsByPath(
            [MarshalAs(UnmanagedType.LPStr)]
            string path
            );

        [DllImport(DLL, CallingConvention = CallingConvention.StdCall)]
        private static extern void _StartIterateComponents();

        [DllImport(DLL, CallingConvention = CallingConvention.StdCall)]
        [return: MarshalAs(UnmanagedType.I1)]
        private static extern bool _CanIterateComponentsNext();

        [DllImport(DLL, CallingConvention = CallingConvention.StdCall)]
        [return: MarshalAs(UnmanagedType.I1)]
        private static extern bool _IterateComponentsNext();

        [DllImport(DLL, CallingConvention = CallingConvention.StdCall)]
        [return: MarshalAs(UnmanagedType.LPStr)]
        private static extern string _GetIteratedComponentId();

        [DllImport(DLL, CallingConvention = CallingConvention.StdCall)]
        private static extern void _EndIterateComponents();

        #endregion
    }
}
