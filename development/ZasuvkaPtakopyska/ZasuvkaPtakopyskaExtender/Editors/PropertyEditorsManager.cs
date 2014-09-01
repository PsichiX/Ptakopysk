using System;
using System.Collections.Generic;
using System.Reflection;
using Newtonsoft.Json.Linq;

namespace ZasuvkaPtakopyskaExtender.Editors
{
    public class PropertyEditorsManager
    {
        private static PropertyEditorsManager m_instance;
        private static readonly char[] m_separators = new char[] { ',', '\t', '\n', '\r' };
        
        private Dictionary<string, Type> m_editors = new Dictionary<string, Type>();
        private Dictionary<string, string> m_aliases = new Dictionary<string, string>();

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

                        if (string.IsNullOrEmpty(a.AliasValueType))
                            RegisterPropertyEditor(a.ValueType, t);
                        else
                            RegisterValueTypeAlias(a.ValueType, a.AliasValueType);
                    }
                }
            }
        }

        public bool RegisterPropertyEditor(string valueType, Type type)
        {
            if (type == null || valueType == null || m_editors.ContainsKey(valueType))
                return false;

            m_editors.Add(valueType, type);
            Console.WriteLine("Register property editor: \"{0}\" for type: {1}", type.FullName, valueType);
            return true;
        }

        public bool RegisterValueTypeAlias(string fromValueType, string toValueType)
        {
            if (!string.IsNullOrEmpty(fromValueType) && !string.IsNullOrEmpty(toValueType))
            {
                if (!m_aliases.ContainsKey(fromValueType))
                {
                    m_aliases.Add(fromValueType, toValueType);
                    return true;
                }
            }
            return false;
        }

        public bool UnregisterPropertyEditor(string valueType)
        {
            if (m_editors.ContainsKey(valueType))
            {
                m_editors.Remove(valueType);
                return true;
            }
            return false;
        }

        public bool UnregisterValueTypeAlias(string fromValueType)
        {
            if (m_aliases.ContainsKey(fromValueType))
            {
                m_aliases.Remove(fromValueType);
                return true;
            }
            return false;
        }

        public void UnregisterAllPropertyEditors()
        {
            m_editors.Clear();
        }

        public void UnregisterAllValueTypeAliases()
        {
            m_aliases.Clear();
        }

        public Type FindPropertyEditor(string valueType)
        {
            if (m_aliases.ContainsKey(valueType))
                valueType = m_aliases[valueType];

            List<Type> genericTypes = new List<Type>();
            if (valueType.StartsWith("@"))
            {
                int tl = valueType.IndexOf('<', 1);
                int a = valueType.IndexOf(':', 1);
                if (tl != -1)
                {
                    string newValueType = "@" + valueType.Substring(1, tl - 1);
                    int tr = valueType.IndexOf('>', tl + 1);
                    if (tr - tl > 1)
                    {
                        string[] gtypes = valueType.Substring(tl + 1, tr - tl - 1).Split(m_separators, StringSplitOptions.RemoveEmptyEntries);
                        if (gtypes != null && gtypes.Length > 0)
                        {
                            Type t;
                            foreach (string gtype in gtypes)
                            {
                                t = Type.GetType(gtype);
                                if (t != null)
                                    genericTypes.Add(t);
                            }
                        }
                    }
                    valueType = newValueType;
                }
                else if (a != -1)
                    valueType = "@" + valueType.Substring(1, a - 1);
            }

            if (m_editors.ContainsKey(valueType))
            {
                if (genericTypes.Count > 0)
                    return m_editors[valueType].MakeGenericType(genericTypes.ToArray());
                else
                    return m_editors[valueType];
            }
            else
                return null;
        }

        public object CreatePropertyEditor(Type editorType, Dictionary<string,string> data, string property, string valueType)
        {
            List<object> args = new List<object>();
            args.Add(data);
            args.Add(property);

            if (valueType.StartsWith("@"))
            {
                int a = valueType.IndexOf(':');
                if (a != -1)
                {
                    try
                    {
                        string json = Utils.EscapedString.Unescape(valueType.Substring(a + 1));
                        JToken d = Newtonsoft.Json.JsonConvert.DeserializeObject<JToken>(json);
                        List<object> additionalArgs = Utils.GenerateAdditionalArguments(editorType, d);
                        if (additionalArgs != null && additionalArgs.Count > 0)
                            args.AddRange(additionalArgs);
                    }
                    catch { }
                }
            }

            return Activator.CreateInstance(editorType, args.ToArray(), null);
        }
    }
}
