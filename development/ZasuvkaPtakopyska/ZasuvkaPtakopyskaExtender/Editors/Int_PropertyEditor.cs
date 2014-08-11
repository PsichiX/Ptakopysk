using System.Collections.Generic;

namespace ZasuvkaPtakopyskaExtender.Editors
{
    [PtakopyskPropertyEditor("int")]
    public class Int_PropertyEditor : ParsablePropertyEditor<int>
    {
        public Int_PropertyEditor(Dictionary<string, string> properties, string propertyName)
            : base(properties, propertyName)
        {
        }
    }
}
