using System;
using System.Collections.Generic;

namespace ZasuvkaPtakopyskaExtender.Editors
{
    [PtakopyskPropertyEditor("@Array")]
    public class ArrayPropertyEditor<T> : CollectionPropertyEditor<T> where T : IFormattable
    {
        public ArrayPropertyEditor(Dictionary<string, string> properties, string propertyName)
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
