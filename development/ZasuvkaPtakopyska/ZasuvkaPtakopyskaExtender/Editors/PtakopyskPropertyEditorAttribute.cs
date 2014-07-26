using System;

namespace ZasuvkaPtakopyskaExtender.Editors
{
    [AttributeUsage(AttributeTargets.Class, AllowMultiple = true)]
    public class PtakopyskPropertyEditorAttribute : Attribute
    {
        public string ValueType { get; set; }
        public int TypePriority { get; set; }

        public PtakopyskPropertyEditorAttribute(string valueType)
        {
            ValueType = valueType;
            TypePriority = 0;
        }
    }
}
