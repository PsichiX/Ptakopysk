using System.Collections.Generic;

namespace ZasuvkaPtakopyskaExtender.Editors
{
    [PtakopyskPropertyEditor("sf::Font")]
    [PtakopyskPropertyEditor("Font", TypePriority = 1)]
    public class FontAsset_PropertyEditor : AssetPropertyEditor
    {
        public FontAsset_PropertyEditor(object propertyOwner, string propertyName)
            : base(propertyOwner, propertyName, null, AssetType.Font)
        {
        }

        public FontAsset_PropertyEditor(Dictionary<string, object> properties, string propertyName)
            : base(properties, propertyName, null, AssetType.Font)
        {
        }
    }
}
