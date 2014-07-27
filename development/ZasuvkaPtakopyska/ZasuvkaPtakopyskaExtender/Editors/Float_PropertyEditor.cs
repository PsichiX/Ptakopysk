using System.Collections.Generic;

namespace ZasuvkaPtakopyskaExtender.Editors
{
    [PtakopyskPropertyEditor("float")]
    public class Float_PropertyEditor : ParsablePropertyEditor<float>
    {
        public Float_PropertyEditor(object propertyOwner, string propertyName)
            : base(propertyOwner, propertyName)
        {
        }
        
        public Float_PropertyEditor(Dictionary<string, object> properties, string propertyName)
            : base(properties, propertyName)
        {
        }
    }
}
