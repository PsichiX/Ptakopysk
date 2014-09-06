using System;
using System.Collections.Generic;

namespace ZasuvkaPtakopyskaExtender.Editors
{
    [PtakopyskPropertyEditor("@StringMapString")]
    public class StringMapStringPropertyEditor : CollectionPropertyEditor<string>
    {
        public StringMapStringPropertyEditor(Dictionary<string, string> properties, string propertyName)
            : base(
            properties,
            propertyName,
            CollectionPropertyEditorUtils.CollectionType.JsonObject,
            (pd, pn) => new String_PropertyEditor(pd, pn)
            )
        {
        }
    }
}
