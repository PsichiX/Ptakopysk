using System.Collections.Generic;

namespace ZasuvkaPtakopyskaExtender.Editors
{
    [PtakopyskPropertyEditor("sf::Shader")]
    [PtakopyskPropertyEditor("Shader", TypePriority = 1)]
    public class ShaderAsset_PropertyEditor : AssetPropertyEditor
    {
        public ShaderAsset_PropertyEditor(object propertyOwner, string propertyName)
            : base(propertyOwner, propertyName, null, AssetType.Shader)
        {
        }

        public ShaderAsset_PropertyEditor(Dictionary<string, object> properties, string propertyName)
            : base(properties, propertyName, null, AssetType.Shader)
        {
        }
    }
}
