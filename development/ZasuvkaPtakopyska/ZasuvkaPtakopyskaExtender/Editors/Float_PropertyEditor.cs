using System.Collections.Generic;

namespace ZasuvkaPtakopyskaExtender.Editors
{
    [PtakopyskPropertyEditor("float")]
    public class Float_PropertyEditor : ParsablePropertyEditor<float>
    {
        public Float_PropertyEditor(Dictionary<string, string> properties, string propertyName)
            : base(properties, propertyName)
        {
        }
    }
}
