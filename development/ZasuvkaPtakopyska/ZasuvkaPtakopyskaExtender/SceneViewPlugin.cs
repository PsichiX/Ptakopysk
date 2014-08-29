using System.Collections.Generic;
using System;

namespace ZasuvkaPtakopyskaExtender
{
    public class SceneViewPlugin
    {
        #region Public Enumerations.

        public enum AssetType
        {
            Texture = 0,
            Shader = 1,
            Sound = 2,
            Music = 3,
            Font = 4,
            CustomAsset = 5
        }

        #endregion



        #region Public Nested Classes.

        public class GameObjectData
        {
            public int handle = 0;
            public string id = "";
            public List<GameObjectData> childs;
        }

        public class AssetsCommonData
        {
            public string id;
            public string meta;
            public List<string> tags;
        }

        #endregion



        #region Private Static Data.

        private static string s_path = null;
        private static bool s_initialized = false;

        #endregion



        #region Public Static Properties.

        public static bool IsLoaded { get { return !string.IsNullOrEmpty(s_path); } }
        public static bool IsInitialized { get { return s_initialized; } }
        
        #endregion



        #region Public Static Functionality.

        public static bool Load(string path)
        {
            if (PluginsInterface.Load(path))
            {
                s_path = path;
                return true;
            }
            else
            {
                s_path = null;
                return false;
            }
        }

        public static bool Unload()
        {
            if (PluginsInterface.Unload(s_path))
            {
                s_path = null;
                s_initialized = false;
                return true;
            }
            return false;
        }

        public static bool Initialize(long windowHandle)
        {
            if (!IsLoaded || !PluginsInterface.SetCurrent(s_path))
                return false;
            var result = PluginsInterface.QueryFunction<bool>("initialize", windowHandle);
            s_initialized = result.result;
            return result.result;
        }

        public static void Release()
        {
            if (!IsLoaded || !PluginsInterface.SetCurrent(s_path))
                return;
            PluginsInterface.QueryFunction<object>("release");
            s_initialized = false;
        }

        public static bool ProcessEvents()
        {
            if (!IsLoaded || !PluginsInterface.SetCurrent(s_path))
                return false;
            var result = PluginsInterface.QueryFunction<bool>("processEvents");
            return result.result;
        }

        public static bool ProcessUpdate(float deltaTime, bool sortInstances)
        {
            if (!IsLoaded || !PluginsInterface.SetCurrent(s_path))
                return false;
            var result = PluginsInterface.QueryFunction<bool>("processUpdate", deltaTime, sortInstances);
            return result.result;
        }

        public static bool ProcessRender()
        {
            if (!IsLoaded || !PluginsInterface.SetCurrent(s_path))
                return false;
            var result = PluginsInterface.QueryFunction<bool>("processRender");
            return result.result;
        }

        public static void SetAssetsFileSystemRoot(string path)
        {
            if (!IsLoaded || !PluginsInterface.SetCurrent(s_path))
                return;
            PluginsInterface.QueryFunction<object>("setAssetsFileSystemRoot", path);
        }

        public static float[] GetGridSize()
        {
            if (!IsLoaded || !PluginsInterface.SetCurrent(s_path))
                return null;
            var result = PluginsInterface.QueryFunction<float[]>("getGridSize");
            return result.result;
        }

        public static void SetGridSize(float[] v)
        {
            if (!IsLoaded || !PluginsInterface.SetCurrent(s_path))
                return;
            PluginsInterface.QueryFunction<object>("setGridSize", v);
        }

        public static void SetGridSize(float x, float y)
        {
            SetGridSize(new float[] { x, y });
        }

        public static float[] GetSceneViewSize()
        {
            if (!IsLoaded || !PluginsInterface.SetCurrent(s_path))
                return null;
            var result = PluginsInterface.QueryFunction<float[]>("getSceneViewSize");
            return result.result;
        }

        public static void SetSceneViewSize(float[] v)
        {
            if (!IsLoaded || !PluginsInterface.SetCurrent(s_path))
                return;
            PluginsInterface.QueryFunction<object>("setSceneViewSize", v);
        }

        public static void SetSceneViewSize(float x, float y)
        {
            SetSceneViewSize(new float[] { x, y });
        }

        public static float[] GetSceneViewCenter()
        {
            if (!IsLoaded || !PluginsInterface.SetCurrent(s_path))
                return null;
            var result = PluginsInterface.QueryFunction<float[]>("getSceneViewCenter");
            return result.result;
        }

        public static void SetSceneViewCenter(float[] v)
        {
            if (!IsLoaded || !PluginsInterface.SetCurrent(s_path))
                return;
            PluginsInterface.QueryFunction<object>("setSceneViewCenter", v);
        }

        public static void SetSceneViewCenter(float x, float y)
        {
            SetSceneViewCenter(new float[] { x, y });
        }

        public static float GetSceneViewZoom()
        {
            if (!IsLoaded || !PluginsInterface.SetCurrent(s_path))
                return 0.0f;
            var result = PluginsInterface.QueryFunction<float>("getSceneViewZoom");
            return result.result;
        }

        public static void SetSceneViewZoom(float v)
        {
            if (!IsLoaded || !PluginsInterface.SetCurrent(s_path))
                return;
            PluginsInterface.QueryFunction<object>("setSceneViewZoom", v);
        }

        public static float[] ConvertPointFromScreenToWorldSpace(int[] p)
        {
            if (!IsLoaded || !PluginsInterface.SetCurrent(s_path))
                return null;
            var result = PluginsInterface.QueryFunction<float[]>("convertPointFromScreenToWorldSpace", p);
            return result.result;
        }

        public static void ConvertPointFromScreenToWorldSpace(int px, int py, out float rx, out float ry)
        {
            float[] r = ConvertPointFromScreenToWorldSpace(new int[] { px, py });
            if (r != null && r.Length == 2)
            {
                rx = r[0];
                ry = r[1];
            }
            else
            {
                rx = 0.0f;
                ry = 0.0f;
            }
        }

        public static bool ClearScene()
        {
            if (!IsLoaded || !PluginsInterface.SetCurrent(s_path))
                return false;
            var result = PluginsInterface.QueryFunction<bool>("clearScene");
            return result.result;
        }

        public static bool ClearSceneGameObjects(bool isPrefab)
        {
            if (!IsLoaded || !PluginsInterface.SetCurrent(s_path))
                return false;
            var result = PluginsInterface.QueryFunction<bool>("clearSceneGameObjects", isPrefab);
            return result.result;
        }

        public static bool ApplyJsonToScene(string json)
        {
            if (!IsLoaded || !PluginsInterface.SetCurrent(s_path))
                return false;
            var root = Newtonsoft.Json.JsonConvert.DeserializeObject(json);
            var result = PluginsInterface.QueryFunction<bool>("applyJsonToScene", root);
            return result.result;
        }

        public static string ConvertSceneToJson()
        {
            if (!IsLoaded || !PluginsInterface.SetCurrent(s_path))
                return null;
            var result = PluginsInterface.QueryFunction<object>("convertSceneToJson");
            string json = Newtonsoft.Json.JsonConvert.SerializeObject(result.result);
            return json;
        }

        public static int CreateGameObject(bool isPrefab, int parent, string prefabSource, string id)
        {
            if (!IsLoaded || !PluginsInterface.SetCurrent(s_path))
                return 0;
            var result = PluginsInterface.QueryFunction<int>("createGameObject", isPrefab, parent, prefabSource, id);
            return result.result;
        }

        public static bool DestroyGameObject(int handle, bool isPrefab)
        {
            if (!IsLoaded || !PluginsInterface.SetCurrent(s_path))
                return false;
            var result = PluginsInterface.QueryFunction<bool>("destroyGameObject", handle, isPrefab);
            return result.result;
        }

        public static bool ClearGameObject(int handle, bool isPrefab)
        {
            if (!IsLoaded || !PluginsInterface.SetCurrent(s_path))
                return false;
            var result = PluginsInterface.QueryFunction<bool>("clearGameObject", handle, isPrefab);
            return result.result;
        }

        public static bool DuplicateGameObject(int handleFrom, bool isPrefabFrom, int handleTo, bool isPrefabTo)
        {
            if (!IsLoaded || !PluginsInterface.SetCurrent(s_path))
                return false;
            var result = PluginsInterface.QueryFunction<bool>("duplicateGameObject", handleFrom, isPrefabFrom, handleTo, isPrefabTo);
            return result.result;
        }

        public static bool TriggerGameObjectComponentFunctionality(int handle, bool isPrefab, string compId, string funcName)
        {
            if (!IsLoaded || !PluginsInterface.SetCurrent(s_path))
                return false;
            var result = PluginsInterface.QueryFunction<bool>("triggerGameObjectComponentFunctionality", handle, isPrefab, compId, funcName);
            return result.result;
        }

        public static bool ApplyJsonToGameObject(int handle, bool isPrefab, string json)
        {
            if (!IsLoaded || !PluginsInterface.SetCurrent(s_path))
                return false;
            var root = Newtonsoft.Json.JsonConvert.DeserializeObject(json);
            var result = PluginsInterface.QueryFunction<bool>("applyJsonToGameObject", handle, isPrefab, root);
            return result.result;
        }

        public static string ConvertGameObjectToJson(int handle, bool isPrefab)
        {
            if (!IsLoaded || !PluginsInterface.SetCurrent(s_path))
                return null;
            var result = PluginsInterface.QueryFunction<object>("convertGameObjectToJson", handle, isPrefab);
            string json = Newtonsoft.Json.JsonConvert.SerializeObject(result.result);
            return json;
        }

        public static int FindGameObjectHandleById(string id, bool isPrefab, int parent)
        {
            if (!IsLoaded || !PluginsInterface.SetCurrent(s_path))
                return 0;
            var result = PluginsInterface.QueryFunction<int>("findGameObjectHandleById", id, isPrefab, parent);
            return result.result;
        }

        public static int FindGameObjectHandleAtScreenPosition(int[] p)
        {
            if (!IsLoaded || !PluginsInterface.SetCurrent(s_path))
                return 0;
            var result = PluginsInterface.QueryFunction<int>("findGameObjectHandleAtScreenPosition", p);
            return result.result;
        }

        public static int FindGameObjectHandleAtScreenPosition(int x, int y)
        {
            return FindGameObjectHandleAtScreenPosition(new int[] { x, y });
        }

        public static List<GameObjectData> ListGameObjects(bool isPrefab)
        {
            if (!IsLoaded || !PluginsInterface.SetCurrent(s_path))
                return null;
            var result = PluginsInterface.QueryFunction<List<GameObjectData>>("listGameObjects", isPrefab);
            return result.result;
        }

        public static Dictionary<string, string> QueryGameObject(int handle, bool isPrefab, string json)
        {
            if (!IsLoaded || !PluginsInterface.SetCurrent(s_path))
                return null;
            var root = Newtonsoft.Json.JsonConvert.DeserializeObject(json);
            var result = PluginsInterface.QueryFunction<Dictionary<string, string>>("queryGameObject", handle, isPrefab, root);
            return result.result;
        }

        public static List<string> ListAssets(AssetType type)
        {
            if (!IsLoaded || !PluginsInterface.SetCurrent(s_path))
                return null;
            var result = PluginsInterface.QueryFunction<List<string>>("listAssets", (int)type);
            return result.result;
        }

        public static List<T> QueryAssets<T>(AssetType type, string json)
        {
            if (!IsLoaded || !PluginsInterface.SetCurrent(s_path))
                return null;
            var root = Newtonsoft.Json.JsonConvert.DeserializeObject(json);
            var result = PluginsInterface.QueryFunction<List<T>>("queryAssets", (int)type, root);
            return result.result;
        }

        public static List<object> QueryAssets(AssetType type, string json)
        {
            return QueryAssets<object>(type, json);
        }

        public static List<T> GetAssets<T>(AssetType type, List<string> ids)
        {
            string json = "{ \"info\": [ ";
            if (ids != null && ids.Count > 0)
            {
                int i = 0;
                foreach (string id in ids)
                {
                    json += "\"" + id + "\"";
                    if (i < ids.Count - 1)
                        json += ", ";
                    i++;
                }
            }
            json += " ] }";
            return QueryAssets<T>(type, json);
        }

        public static List<object> GetAssets(AssetType type, List<string> ids)
        {
            return GetAssets<object>(type, ids);
        }

        public static List<string> ListComponents()
        {
            if (!IsLoaded || !PluginsInterface.SetCurrent(s_path))
                return null;
            var result = PluginsInterface.QueryFunction<List<string>>("listComponents");
            return result.result;
        }

        public static List<string> ListCustomAssets()
        {
            if (!IsLoaded || !PluginsInterface.SetCurrent(s_path))
                return null;
            var result = PluginsInterface.QueryFunction<List<string>>("listCustomAssets");
            return result.result;
        }

        #endregion
    }
}
