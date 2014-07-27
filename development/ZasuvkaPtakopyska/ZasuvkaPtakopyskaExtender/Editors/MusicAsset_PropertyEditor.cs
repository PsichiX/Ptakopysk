using System.Collections.Generic;

namespace ZasuvkaPtakopyskaExtender.Editors
{
    [PtakopyskPropertyEditor("sf::Music")]
    [PtakopyskPropertyEditor("Music", TypePriority = 1)]
    public class MusicAsset_PropertyEditor : AssetPropertyEditor
    {
        public MusicAsset_PropertyEditor(object propertyOwner, string propertyName)
            : base(propertyOwner, propertyName, null, AssetType.Music)
        {
        }

        public MusicAsset_PropertyEditor(Dictionary<string, object> properties, string propertyName)
            : base(properties, propertyName, null, AssetType.Music)
        {
        }
    }
}
