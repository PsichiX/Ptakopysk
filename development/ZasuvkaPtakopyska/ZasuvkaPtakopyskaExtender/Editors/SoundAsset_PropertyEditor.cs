using System.Collections.Generic;

namespace ZasuvkaPtakopyskaExtender.Editors
{
    [PtakopyskPropertyEditor("sf::Sound")]
    [PtakopyskPropertyEditor("Sound", TypePriority = 1)]
    public class SoundAsset_PropertyEditor : AssetPropertyEditor
    {
        public SoundAsset_PropertyEditor(Dictionary<string, string> properties, string propertyName)
            : base(properties, propertyName, PtakopyskInterface.AssetType.Sound)
        {
        }
    }
}
