using System.Collections.Generic;

namespace ZasuvkaPtakopyskaExtender.Editors
{
    [PtakopyskPropertyEditor("sf::Sound")]
    [PtakopyskPropertyEditor("Sound", TypePriority = 1)]
    public class SoundAsset_PropertyEditor : AssetPropertyEditor
    {
        public SoundAsset_PropertyEditor(object propertyOwner, string propertyName)
            : base(propertyOwner, propertyName, null, AssetType.Sound)
        {
        }

        public SoundAsset_PropertyEditor(Dictionary<string, object> properties, string propertyName)
            : base(properties, propertyName, null, AssetType.Sound)
        {
        }
    }
}
