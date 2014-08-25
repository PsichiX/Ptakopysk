using System.Collections.Generic;
using PtakopyskMetaGenerator;

namespace ZasuvkaPtakopyskaExtender
{
    public class MetaAssetsManager
    {
        private static MetaAssetsManager m_instance;

        private List<MetaAsset> m_metaAssets = new List<MetaAsset>();

        public static MetaAssetsManager Instance
        {
            get
            {
                if (m_instance == null)
                    m_instance = new MetaAssetsManager();
                return m_instance;
            }
        }

        public bool RegisterMetaAsset(MetaAsset meta)
        {
            if (meta == null || m_metaAssets.Exists(item => item.Name == meta.Name))
                return false;

            m_metaAssets.Add(meta);
            return true;
        }

        public bool UnregisterMetaAsset(MetaAsset meta)
        {
            return m_metaAssets.Remove(meta);
        }

        public bool UnregisterMetaAsset(string name)
        {
            return m_metaAssets.RemoveAll(item => item.Name == name) > 0;
        }

        public void UnregisterAllMetaAssets()
        {
            m_metaAssets.Clear();
        }

        public MetaAsset FindMetaAsset(string name)
        {
            return m_metaAssets.Find(item => item.Name == name);
        }
    }
}
