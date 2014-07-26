using System.Collections.Generic;

namespace ZasuvkaPtakopyskaExtender.Editors
{
    [PtakopyskPropertyEditor("float")]
    public class FloatPropertyEditor : ParsablePropertyEditor<float>
    {
        public FloatPropertyEditor(object propertyOwner, string propertyName)
            : base(propertyOwner, propertyName, 0.0f)
        {
        }
        
        public FloatPropertyEditor(Dictionary<string, object> properties, string propertyName)
            : base(properties, propertyName, 0.0f)
        {
        }
    }
}
