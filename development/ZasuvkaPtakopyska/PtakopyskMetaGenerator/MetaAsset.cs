namespace PtakopyskMetaGenerator
{
    public class MetaAsset
    {
        private string m_type = "Asset";

        public string Type { get { return m_type; } }
        public string Name { get; set; }
        public string Description { get; set; }

        public MetaAsset(string name = null)
        {
            Name = name;
            Description = "";
        }
    }
}
