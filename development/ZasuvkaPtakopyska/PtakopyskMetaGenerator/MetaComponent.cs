using System.Collections.Generic;

namespace PtakopyskMetaGenerator
{
    public class MetaComponent
    {
        private string m_type = "Component";

        public string Type { get { return m_type; } }
        public string Name { get; set; }
        public string Description { get; set; }
        public List<string> BaseClasses { get; set; }
        public List<MetaProperty> Properties { get; set; }
        public List<string> FunctionalityTriggers { get; set; }

        public MetaComponent(string name = null)
        {
            Name = name;
            Description = "";
            BaseClasses = new List<string>();
            Properties = new List<MetaProperty>();
            FunctionalityTriggers = new List<string>();
        }
    }
}
