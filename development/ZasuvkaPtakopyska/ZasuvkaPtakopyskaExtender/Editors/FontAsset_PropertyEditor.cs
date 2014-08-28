using System.Collections.Generic;

namespace ZasuvkaPtakopyskaExtender.Editors
{
    [PtakopyskPropertyEditor("sf::Font")]
    [PtakopyskPropertyEditor("Font", TypePriority = 1)]
    public class FontAsset_PropertyEditor : AssetPropertyEditor
    {
        public FontAsset_PropertyEditor(Dictionary<string, string> properties, string propertyName)
            : base(properties, propertyName, SceneViewPlugin.AssetType.Font)
        {
        }
    }
}
