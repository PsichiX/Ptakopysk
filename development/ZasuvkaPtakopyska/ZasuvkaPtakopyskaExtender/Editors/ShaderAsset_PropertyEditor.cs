using System.Collections.Generic;

namespace ZasuvkaPtakopyskaExtender.Editors
{
    [PtakopyskPropertyEditor("sf::Shader")]
    [PtakopyskPropertyEditor("Shader", TypePriority = 1)]
    public class ShaderAsset_PropertyEditor : AssetPropertyEditor
    {
        public ShaderAsset_PropertyEditor(Dictionary<string, string> properties, string propertyName)
            : base(properties, propertyName, SceneViewPlugin.AssetType.Shader)
        {
        }
    }
}
