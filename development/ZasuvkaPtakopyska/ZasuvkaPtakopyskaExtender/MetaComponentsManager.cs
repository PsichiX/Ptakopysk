using System.Collections.Generic;
using PtakopyskMetaGenerator;

namespace ZasuvkaPtakopyskaExtender
{
    public class MetaComponentsManager
    {
        private static MetaComponentsManager m_instance;

        private List<MetaComponent> m_metaComponents = new List<MetaComponent>();

        public static MetaComponentsManager Instance
        {
            get
            {
                if (m_instance == null)
                    m_instance = new MetaComponentsManager();
                return m_instance;
            }
        }

        public bool RegisterMetaComponent(MetaComponent meta)
        {
            if (meta == null || m_metaComponents.Exists(item => item.Name == meta.Name))
                return false;

            m_metaComponents.Add(meta);
            return true;
        }

        public bool UnregisterMetaComponent(MetaComponent meta)
        {
            return m_metaComponents.Remove(meta);
        }

        public bool UnregisterMetaComponent(string name)
        {
            return m_metaComponents.RemoveAll(item => item.Name == name) > 0;
        }

        public void UnregisterAllMetaComponents()
        {
            m_metaComponents.Clear();
        }

        public MetaComponent FindMetaComponent(string name)
        {
            return m_metaComponents.Find(item => item.Name == name);
        }

        public List<MetaProperty> GetFlattenPropertiesOf(MetaComponent component)
        {
            if (component == null)
                return null;

            List<MetaProperty> result = new List<MetaProperty>();

            MetaComponent baseMeta;
            List<MetaProperty> baseProps;
            if (component.BaseClasses != null && component.BaseClasses.Count > 0)
            {
                foreach (string baseClass in component.BaseClasses)
                {
                    baseMeta = FindMetaComponent(baseClass);
                    if (baseMeta == null)
                        continue;

                    baseProps = GetFlattenPropertiesOf(baseMeta);
                    if (baseProps == null || baseProps.Count == 0)
                        continue;

                    foreach (MetaProperty p in baseProps)
                        if (!result.Exists(item => item.Name == p.Name))
                            result.Add(p);
                }
            }

            foreach (MetaProperty p in component.Properties)
                if (!result.Exists(item => item.Name == p.Name))
                    result.Add(p);

            return result;
        }
    }
}
