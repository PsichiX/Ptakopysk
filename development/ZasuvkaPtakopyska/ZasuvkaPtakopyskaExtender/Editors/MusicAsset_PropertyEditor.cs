using System.Collections.Generic;

namespace ZasuvkaPtakopyskaExtender.Editors
{
    [PtakopyskPropertyEditor("sf::Music")]
    [PtakopyskPropertyEditor("Music", TypePriority = 1)]
    public class MusicAsset_PropertyEditor : AssetPropertyEditor
    {
        public MusicAsset_PropertyEditor(Dictionary<string, string> properties, string propertyName)
            : base(properties, propertyName, SceneViewPlugin.AssetType.Music)
        {
        }
    }
}
