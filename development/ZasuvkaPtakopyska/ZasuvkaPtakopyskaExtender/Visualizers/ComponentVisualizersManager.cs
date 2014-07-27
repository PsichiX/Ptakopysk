using System;
using System.Collections.Generic;
using System.Reflection;

namespace ZasuvkaPtakopyskaExtender.Visualizers
{
    public class ComponentVisualizersManager
    {
        private static ComponentVisualizersManager m_instance;

        private Dictionary<string, Type> m_visualizers = new Dictionary<string, Type>();

        public static ComponentVisualizersManager Instance
        {
            get
            {
                if (m_instance == null)
                    m_instance = new ComponentVisualizersManager();
                return m_instance;
            }
        }

        public void RegisterComponentVisualizersFromAssembly(Assembly assembly)
        {
            if (assembly == null)
                return;

            object[] attributes = assembly.GetCustomAttributes(typeof(ZasuvkaPtakopyskaExtenderAttribute), false);
            if (attributes == null)
                return;

            foreach (Type t in assembly.GetTypes())
            {
                attributes = t.GetCustomAttributes(typeof(PtakopyskComponentVisualizerAttribute), false);
                if (attributes != null && attributes.Length > 0)
                {
                    PtakopyskComponentVisualizerAttribute a;
                    foreach (object attr in attributes)
                    {
                        a = attr as PtakopyskComponentVisualizerAttribute;
                        if (a == null)
                            continue;

                        RegisterComponentVisualizer(a.ComponentType, t);
                    }
                }
            }
        }

        public bool RegisterComponentVisualizer<T>(string componentType) where T : IComponentVisualizer
        {
            if (componentType == null)
                return false;

            Type t = typeof(T);
            if (m_visualizers.ContainsKey(componentType))
                return false;

            m_visualizers.Add(componentType, t);
            Console.WriteLine("Register component visualizer: \"{0}\" for type: {1}", t.FullName, componentType);
            return true;
        }

        public bool RegisterComponentVisualizer(string componentType, Type type)
        {
            if (type == null || componentType == null || m_visualizers.ContainsKey(componentType))
                return false;

            m_visualizers.Add(componentType, type);
            Console.WriteLine("Register component visualizer: \"{0}\" for type: {1}", type.FullName, componentType);
            return true;
        }

        public bool UnregisterComponentVisualizer<T>() where T : IComponentVisualizer
        {
            Type t = typeof(T);
            bool status = false;
            foreach (string key in m_visualizers.Keys)
            {
                if (m_visualizers[key] == t)
                {
                    m_visualizers.Remove(key);
                    status = true;
                }
            }
            return status;
        }

        public bool UnregisterComponentVisualizerByComponentType(string componentType)
        {
            if (m_visualizers.ContainsKey(componentType))
            {
                m_visualizers.Remove(componentType);
                return true;
            }
            return false;
        }

        public void UnregisterAllComponentVisualizers()
        {
            m_visualizers.Clear();
        }

        public Type FindComponentVisualizer<T>() where T : IComponentVisualizer
        {
            Type t = typeof(T);
            foreach (string key in m_visualizers.Keys)
                if (m_visualizers[key] == t)
                    return m_visualizers[key];
            return null;
        }

        public Type FindComponentVisualizerByComponentType(string componentType)
        {
            return m_visualizers.ContainsKey(componentType) ? m_visualizers[componentType] : null;
        }
    }
}
