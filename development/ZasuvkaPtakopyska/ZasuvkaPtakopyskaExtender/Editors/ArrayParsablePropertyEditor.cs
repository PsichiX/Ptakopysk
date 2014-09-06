using System;
using System.Collections.Generic;

namespace ZasuvkaPtakopyskaExtender.Editors
{
    [PtakopyskPropertyEditor("@ArrayParsable")]
    public class ArrayParsablePropertyEditor<T> : CollectionPropertyEditor<T> where T : IFormattable
    {
        public ArrayParsablePropertyEditor(Dictionary<string, string> properties, string propertyName)
            : base(
            properties,
            propertyName,
            CollectionPropertyEditorUtils.CollectionType.JsonArray,
            (pd, pn) => new ParsablePropertyEditor<T>(pd, pn)
            )
        {
        }
    }
}
