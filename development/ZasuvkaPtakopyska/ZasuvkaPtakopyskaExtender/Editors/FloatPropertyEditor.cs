namespace ZasuvkaPtakopyskaExtender.Editors
{
    [PtakopyskPropertyEditor("float")]
    public class FloatPropertyEditor : ParsablePropertyEditor<float>
    {
        public FloatPropertyEditor(object propertyOwner, string propertyName)
            : base(propertyOwner, propertyName, 0.0f)
        {
        }
    }
}
