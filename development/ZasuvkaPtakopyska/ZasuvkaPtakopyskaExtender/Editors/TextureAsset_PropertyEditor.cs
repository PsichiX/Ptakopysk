using System.Collections.Generic;

namespace ZasuvkaPtakopyskaExtender.Editors
{
    [PtakopyskPropertyEditor("sf::Texture")]
    [PtakopyskPropertyEditor("Texture", TypePriority = 1)]
    public class TextureAsset_PropertyEditor : AssetPropertyEditor
    {
        public TextureAsset_PropertyEditor(Dictionary<string, string> properties, string propertyName)
            : base(properties, propertyName, SceneViewPlugin.AssetType.Texture)
        {
        }
    }
}
