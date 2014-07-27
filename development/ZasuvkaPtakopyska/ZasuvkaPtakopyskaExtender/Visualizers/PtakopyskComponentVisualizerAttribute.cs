using System;

namespace ZasuvkaPtakopyskaExtender.Visualizers
{
    [AttributeUsage(AttributeTargets.Class, AllowMultiple = true)]
    public class PtakopyskComponentVisualizerAttribute : Attribute
    {
        public string ComponentType { get; set; }
        public int TypePriority { get; set; }
        
        public PtakopyskComponentVisualizerAttribute(string componentType)
        {
            ComponentType = componentType;
        }
    }
}
