using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ZasuvkaPtakopyskaExtender.Editors
{
    [PtakopyskPropertyEditor("Body::VerticesData")]
    [PtakopyskPropertyEditor("VerticesData", TypePriority = 1)]
    public class FloatVector2Collection_PropertyEditor : CollectionPropertyEditor<List<float>>
    {
        public FloatVector2Collection_PropertyEditor(Dictionary<string, string> properties, string propertyName)
            : base(
            properties,
            propertyName,
            CollectionPropertyEditorUtils.CollectionType.JsonArray,
            (pd, pn) => new FloatVector2_PropertyEditor(pd, pn)
            )
        {
        }
    }
}
