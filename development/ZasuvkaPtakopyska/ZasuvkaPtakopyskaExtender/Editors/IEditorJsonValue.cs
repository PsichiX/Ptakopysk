namespace ZasuvkaPtakopyskaExtender.Editors
{
    public interface IEditorJsonValue
    {
        string JsonDefaultValue { get; set; }
        string JsonValue { get; set; }

        void UpdateEditorValue();
    }
}
