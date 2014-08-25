using System.Collections.Generic;

namespace ZasuvkaPtakopyskaExtender.Editors
{
    [PtakopyskPropertyEditor("ICustomAsset")]
    public class CustomAsset_PropertyEditor : AssetPropertyEditor
    {
        public CustomAsset_PropertyEditor(Dictionary<string, string> properties, string propertyName)
            : base(properties, propertyName, PtakopyskInterface.AssetType.CustomAsset)
        {
        }
    }
}
