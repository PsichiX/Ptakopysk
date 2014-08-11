using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ZasuvkaPtakopyskaExtender.Editors
{
    public class AssetPropertyEditor : EnumPropertyEditor
    {
        private static readonly string NONE = "";

        private PtakopyskInterface.AssetType m_assetType;

        public PtakopyskInterface.AssetType AssetsType { get { return m_assetType; } set { m_assetType = value; UpdateValuesSource(); } }
        
        public AssetPropertyEditor(Dictionary<string, string> properties, string propertyName, PtakopyskInterface.AssetType assetType)
            : base(properties, propertyName, null)
        {
            m_assetType = assetType;
            UpdateValuesSource();
        }

        private void UpdateValuesSource()
        {
            List<string> values = new List<string>();
            values.Add(NONE);
            PtakopyskInterface.Instance.StartIterateAssets(m_assetType);
            string id;
            while (PtakopyskInterface.Instance.CanIterateAssetsNext(m_assetType))
            {
                id = PtakopyskInterface.Instance.GetIteratedAssetId(m_assetType);
                if (!string.IsNullOrEmpty(id))
                    values.Add(id);
                PtakopyskInterface.Instance.IterateAssetsNext(m_assetType);
            }
            PtakopyskInterface.Instance.EndIterateAssets(m_assetType);
            ValuesSource = values.ToArray();
            UpdateEditorValue();
        }
    }
}
