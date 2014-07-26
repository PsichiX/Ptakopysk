using System.Collections.Generic;

namespace ZasuvkaPtakopyskaExtender.Editors
{
    [PtakopyskPropertyEditor("int")]
    public class IntPropertyEditor : ParsablePropertyEditor<int>
    {
        public IntPropertyEditor(object propertyOwner, string propertyName)
            : base(propertyOwner, propertyName, 0)
        {
        }

        public IntPropertyEditor(Dictionary<string, object> properties, string propertyName)
            : base(properties, propertyName, 0)
        {
        }
    }
}
