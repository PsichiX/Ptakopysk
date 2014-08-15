using System.Collections.Generic;

namespace ZasuvkaPtakopyskaExtender.Editors
{
    [PtakopyskPropertyEditor("b2BodyType")]
    public class BodyType_PropertyEditor : EnumPropertyEditor
    {
        private static readonly string[] VALUES = new string[] { "b2_staticBody", "b2_kinematicBody", "b2_dynamicBody" };

        public BodyType_PropertyEditor(Dictionary<string, string> properties, string propertyName)
            : base(properties, propertyName, VALUES)
        {
        }
    }
}
