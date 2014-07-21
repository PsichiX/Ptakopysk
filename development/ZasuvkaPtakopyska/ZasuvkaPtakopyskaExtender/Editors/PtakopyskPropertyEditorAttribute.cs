using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ZasuvkaPtakopyskaExtender.Editors
{
    [AttributeUsage(AttributeTargets.Class, AllowMultiple = true)]
    public class PtakopyskPropertyEditorAttribute : Attribute
    {
        public string ValueType { get; set; }

        public PtakopyskPropertyEditorAttribute(string valueType)
        {
            ValueType = valueType;
        }
    }
}
