using System.Collections.Generic;

namespace ZasuvkaPtakopyskaExtender.Editors
{
    [PtakopyskPropertyEditor("sf::Texture")]
    [PtakopyskPropertyEditor("Texture", TypePriority = 1)]
    public class TextureAsset_PropertyEditor : AssetPropertyEditor
    {
        public TextureAsset_PropertyEditor(object propertyOwner, string propertyName)
            : base(propertyOwner, propertyName, null, AssetType.Texture)
        {
        }

        public TextureAsset_PropertyEditor(Dictionary<string, object> properties, string propertyName)
            : base(properties, propertyName, null, AssetType.Texture)
        {
        }
    }
}
