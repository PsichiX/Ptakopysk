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

        virtual protected List<string> OnUpdateValuesSource(List<string> ids)
        {
            return ids;
        }

        protected void UpdateValuesSource()
        {
            List<string> values = SceneViewPlugin.ListAssets(m_assetType);
            values = OnUpdateValuesSource(values);
            if (values != null)
            {
                values.Insert(0, NONE);
                ValuesSource = values.ToArray();
                UpdateEditorValue();
            }
            else
            {
                ValuesSource = new string[]{};
                UpdateEditorValue();
            }
        }
    }
}
