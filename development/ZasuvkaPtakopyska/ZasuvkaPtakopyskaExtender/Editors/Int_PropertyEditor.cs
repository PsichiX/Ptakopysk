using System.Collections.Generic;

namespace ZasuvkaPtakopyskaExtender.Editors
{
    [PtakopyskPropertyEditor("int")]
    public class Int_PropertyEditor : ParsablePropertyEditor<int>
    {
        public Int_PropertyEditor(object propertyOwner, string propertyName)
            : base(propertyOwner, propertyName)
        {
        }

        public Int_PropertyEditor(Dictionary<string, object> properties, string propertyName)
            : base(properties, propertyName)
        {
        }
    }
}
