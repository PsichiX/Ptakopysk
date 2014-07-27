using System;
using System.Collections.Generic;
using System.Reflection;

namespace ZasuvkaPtakopyskaExtender.Editors
{
    public class PropertyEditorsManager
    {
        private static PropertyEditorsManager m_instance;

        private Dictionary<string, Type> m_editors = new Dictionary<string, Type>();

        public static PropertyEditorsManager Instance
        {
            get
            {
                if (m_instance == null)
                    m_instance = new PropertyEditorsManager();
                return m_instance;
            }
        }

        public void RegisterPropertyEditorsFromAssembly(Assembly assembly)
        {
            if (assembly == null)
                return;
            
            object[] attributes = assembly.GetCustomAttributes(typeof(ZasuvkaPtakopyskaExtenderAttribute), false);
            if (attributes == null)
                return;

            foreach (Type t in assembly.GetTypes())
            {
                attributes = t.GetCustomAttributes(typeof(PtakopyskPropertyEditorAttribute), false);
                if (attributes != null && attributes.Length > 0)
                {
                    PtakopyskPropertyEditorAttribute a;
                    foreach (object attr in attributes)
                    {
                        a = attr as PtakopyskPropertyEditorAttribute;
                        if (a == null)
                            continue;

                        RegisterPropertyEditor(a.ValueType, t);
                    }
                }
            }
        }

        public bool RegisterPropertyEditor<T>(string valueType) where T : PropertyEditor<T>
        {
            if (valueType == null)
                return false;

            Type t = typeof(T);
            if (m_editors.ContainsKey(valueType))
                return false;

            m_editors.Add(valueType, t);
            Console.WriteLine("Register property editor: \"{0}\" for type: {1}", t.FullName, valueType);
            return true;
        }

        public bool RegisterPropertyEditor(string valueType, Type type)
        {
            if (type == null || valueType == null || m_editors.ContainsKey(valueType))
                return false;

            m_editors.Add(valueType, type);
            Console.WriteLine("Register property editor: \"{0}\" for type: {1}", type.FullName, valueType);
            return true;
        }

        public bool UnregisterPropertyEditor<T>() where T : PropertyEditor<T>
        {
            Type t = typeof(T);
            bool status = false;
            foreach (string key in m_editors.Keys)
            {
                if (m_editors[key] == t)
                {
                    m_editors.Remove(key);
                    status = true;
                }
            }
            return status;
        }

        public bool UnregisterPropertyEditorByValueType(string valueType)
        {
            if (m_editors.ContainsKey(valueType))
            {
                m_editors.Remove(valueType);
                return true;
            }
            return false;
        }

        public void UnregisterAllPropertyEditors()
        {
            m_editors.Clear();
        }

        public Type FindPropertyEditor<T>() where T : PropertyEditor<T>
        {
            Type t = typeof(T);
            foreach (string key in m_editors.Keys)
                if (m_editors[key] == t)
                    return m_editors[key];
            return null;
        }

        public Type FindPropertyEditorByValueType(string valueType)
        {
            return m_editors.ContainsKey(valueType) ? m_editors[valueType] : null;
        }
    }
}
