namespace ZasuvkaPtakopyskaExtender.Editors
{
    [PtakopyskPropertyEditor("int")]
    public class IntPropertyEditor : ParsablePropertyEditor<int>
    {
        public IntPropertyEditor(object propertyOwner, string propertyName)
            : base(propertyOwner, propertyName, 0)
        {
        }
    }
}
