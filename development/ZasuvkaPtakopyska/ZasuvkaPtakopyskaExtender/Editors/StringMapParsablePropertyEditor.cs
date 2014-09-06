using System;
using System.Collections.Generic;

namespace ZasuvkaPtakopyskaExtender.Editors
{
    [PtakopyskPropertyEditor("@StringMapParsable")]
    public class StringMapParsablePropertyEditor<T> : CollectionPropertyEditor<T> where T : IFormattable
    {
        public StringMapParsablePropertyEditor(Dictionary<string, string> properties, string propertyName)
            : base(
            properties,
            propertyName,
            CollectionPropertyEditorUtils.CollectionType.JsonObject,
            (pd, pn) => new ParsablePropertyEditor<T>(pd, pn)
            )
        {
        }
    }
}
