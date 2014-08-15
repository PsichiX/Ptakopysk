using System.Collections.Generic;

namespace PtakopyskMetaGenerator
{
    public class MetaComponent
    {
        public string Name { get; set; }
        public string Description { get; set; }
        public List<string> BaseClasses { get; set; }
        public List<MetaProperty> Properties { get; set; }
        public List<string> FunctionalityTriggers { get; set; }

        public MetaComponent(string name = null)
        {
            Name = name;
            BaseClasses = new List<string>();
            Properties = new List<MetaProperty>();
            FunctionalityTriggers = new List<string>();
        }
    }
}
