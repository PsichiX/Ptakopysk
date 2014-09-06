using System.Collections.Generic;
using Newtonsoft.Json.Linq;

namespace ZasuvkaPtakopyskaExtender.Editors
{
    [PtakopyskPropertyEditor("@CustomAsset")]
    public class CustomAsset_PropertyEditor : AssetPropertyEditor
    {
        private class Info
        {
            public string id = "";
            public string type = "";
        }

        private string m_type;

        new public static List<object> GenerateAdditionalArguments(JToken data)
        {
            if (data == null || data.Type != JTokenType.String)
                return null;

            List<object> result = new List<object>();
            result.Add(data.ToObject<string>());
            return result;
        }

        public CustomAsset_PropertyEditor(Dictionary<string, string> properties, string propertyName, string customAssetType = "")
            : base(properties, propertyName, SceneViewPlugin.AssetType.CustomAsset)
        {
            m_type = customAssetType;
            UpdateValuesSource();
        }

        override protected List<string> OnUpdateValuesSource(List<string> ids)
        {
            if (ids == null || string.IsNullOrEmpty(m_type))
                return ids;

            List<string> result = new List<string>();
            List<Info> assets = SceneViewPlugin.GetAssets<Info>(AssetsType, ids);
            foreach (Info info in assets)
                if (info.type == m_type)
                    result.Add(info.id);
            return result;
        }
    }
}
