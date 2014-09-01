using System;
using System.Collections.Generic;

namespace ZasuvkaPtakopyskaExtender.Editors
{
    [PtakopyskPropertyEditor("@Mesh")]
    public class MeshPropertyEditor<T> : CollectionPropertyEditor<List<T>> where T : IFormattable
    {
        public MeshPropertyEditor(Dictionary<string, string> properties, string propertyName)
            : base(
            properties,
            propertyName,
            CollectionPropertyEditorUtils.CollectionType.JsonArray,
            (pd, pn) => new Vector2PropertyEditor<T>(pd, pn)
            )
        {
        }
    }
}
