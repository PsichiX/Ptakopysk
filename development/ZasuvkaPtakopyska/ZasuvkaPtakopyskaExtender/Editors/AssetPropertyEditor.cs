using System.Collections.Generic;

namespace ZasuvkaPtakopyskaExtender.Editors
{
    public class AssetPropertyEditor : EnumPropertyEditor
    {
        private static readonly string NONE = "";

        private SceneViewPlugin.AssetType m_assetType;

        public SceneViewPlugin.AssetType AssetsType { get { return m_assetType; } set { m_assetType = value; UpdateValuesSource(); } }

        public AssetPropertyEditor(Dictionary<string, string> properties, string propertyName, SceneViewPlugin.AssetType assetType)
            : base(properties, propertyName, null)
        {
            m_assetType = assetType;
            UpdateValuesSource();
        }

        private void UpdateValuesSource()
        {
            List<string> values = SceneViewPlugin.ListAssets(m_assetType);
            values.Add(NONE);
            ValuesSource = values.ToArray();
            UpdateEditorValue();
        }
    }
}
