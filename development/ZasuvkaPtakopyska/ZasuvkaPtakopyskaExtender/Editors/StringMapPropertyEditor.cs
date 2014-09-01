using System;
using System.Collections.Generic;

namespace ZasuvkaPtakopyskaExtender.Editors
{
    [PtakopyskPropertyEditor("@StringMap")]
    public class StringMapPropertyEditor<T> : CollectionPropertyEditor<T> where T : IFormattable
    {
        public StringMapPropertyEditor(Dictionary<string, string> properties, string propertyName)
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
