using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Newtonsoft.Json;

namespace ZasuvkaPtakopyska
{
    public class SceneModel
    {
        public class Physics
        {
            public class b2Filter
            {
                public UInt16 categoryBits { get; set; }
                public UInt16 maskBits { get; set; }
                public Int16 groupIndex { get; set; }
            }

            public List<float> gravity { get; set; }
            public Dictionary<string, b2Filter> filters { get; set; }
        }

        public class Assets
        {
            public class Texture
            {
                public string id { get; set; }
                public string path { get; set; }
                public List<string> tags { get; set; }
            }

            public class Shader
            {
                public string id { get; set; }
                public string vspath { get; set; }
                public string fspath { get; set; }
                public List<string> tags { get; set; }
                public List<string> uniforms { get; set; }
            }

            public class Sound
            {
                public string id { get; set; }
                public string path { get; set; }
                public List<string> tags { get; set; }
            }

            public class Music
            {
                public string id { get; set; }
                public string path { get; set; }
                public List<string> tags { get; set; }
            }

            public class Font
            {
                public string id { get; set; }
                public string path { get; set; }
                public List<string> tags { get; set; }
            }

            public List<Texture> textures { get; set; }
            public List<Shader> shaders { get; set; }
            public List<Sound> sounds { get; set; }
            public List<Music> musics { get; set; }
            public List<Font> fonts { get; set; }
        }

        public class GameObject
        {
            public class Properties
            {
                public string Id { get; set; }
                public bool Active { get; set; }
                public int Order { get; set; }
                public object MetaData { get; set; }

                public Properties()
                {
                    Active = true;
                }

                public void ApplyFrom(Properties props)
                {
                    if (props == null)
                        return;

                    if (props.Id != null)
                        Id = props.Id;
                    Active = props.Active;
                    Order = props.Order;
                    MetaData = props.MetaData;
                }
            }

            public class Component
            {
                public string type { get; set; }
                public Dictionary<string, object> properties { get; set; }

                public void ApplyFrom(Component comp)
                {
                    if (comp == null)
                        return;

                    type = comp.type;
                    if (comp.properties != null)
                    {
                        if (properties == null)
                            properties = new Dictionary<string, object>();
                        foreach (string k in comp.properties.Keys)
                            properties[k] = comp.properties[k];
                    }
                }
            }

            public string prefab { get; set; }
            public Properties properties { get; set; }
            public List<Component> components { get; set; }
            public List<GameObject> gameObjects { get; set; }

            public void ApplyFrom(GameObject go)
            {
                if (go == null)
                    return;

                if (go.properties != null)
                {
                    if (properties == null)
                        properties = new Properties();
                    properties.ApplyFrom(go.properties);
                }
                if (go.components != null)
                {
                    if (components == null)
                        components = new List<Component>();
                    Component c;
                    Component comp;
                    Component _comp;
                    for (int i = 0; i < go.components.Count; ++i)
                    {
                        c = go.components[i];
                        comp = components.Find(item => item.type == c.type);
                        if (comp != null)
                        {
                            _comp = new Component();
                            _comp.ApplyFrom(c);
                            _comp.ApplyFrom(comp);
                            components[i] = _comp;
                        }
                        else
                        {
                            _comp = new Component();
                            _comp.ApplyFrom(c);
                            components.Add(_comp);
                        }
                    }
                }
                if (go.gameObjects != null)
                {
                    if (gameObjects == null)
                        gameObjects = new List<GameObject>();
                    GameObject g;
                    GameObject obj;
                    GameObject _obj;
                    for (int i = 0; i < go.gameObjects.Count; ++i)
                    {
                        g = go.gameObjects[i];
                        if (g.properties == null)
                            continue;

                        obj = gameObjects.Find(item => item.properties != null && item.properties.Id == g.properties.Id);
                        if (obj != null)
                        {
                            _obj = new GameObject();
                            _obj.ApplyFrom(g);
                            _obj.ApplyFrom(obj);
                            gameObjects[i] = _obj;
                        }
                        else
                        {
                            _obj = new GameObject();
                            _obj.ApplyFrom(g);
                            gameObjects.Add(_obj);
                        }
                    }
                }
            }
        }

        public Physics physics { get; set; }
        public Assets assets { get; set; }
        public List<GameObject> prefabs { get; set; }
        public List<GameObject> scene { get; set; }

        public void PostProcessScene()
        {
            if (scene != null && prefabs != null)
            {
                GameObject go;
                GameObject _go;
                for (int i = 0; i < scene.Count; ++i)
                {
                    go = scene[i];
                    if (!String.IsNullOrEmpty(go.prefab))
                    {
                        GameObject prefab = prefabs.Find(item => item.properties != null && item.properties.Id == go.prefab);
                        if (prefab != null)
                        {
                            _go = new GameObject();
                            _go.ApplyFrom(prefab);
                            _go.ApplyFrom(go);
                            scene[i] = _go;
                        }
                    }
                }
            }
        }
    }
}
