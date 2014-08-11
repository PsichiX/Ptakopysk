using System.Collections.Generic;

namespace ZasuvkaPtakopyskaExtender.Editors
{
    [PtakopyskPropertyEditor("Transform::ModeType")]
    [PtakopyskPropertyEditor("ModeType", TypePriority = 1)]
    public class TransformModeType_PropertyEditor : EnumPropertyEditor
    {
        private static readonly string[] VALUES = new string[] { "mHierarchy", "mParent", "mGlobal" };

        public TransformModeType_PropertyEditor(Dictionary<string, string> properties, string propertyName)
            : base(properties, propertyName, VALUES)
        {
        }
    }
}
