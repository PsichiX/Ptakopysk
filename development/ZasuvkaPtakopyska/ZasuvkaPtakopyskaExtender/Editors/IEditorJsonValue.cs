namespace ZasuvkaPtakopyskaExtender.Editors
{
    public interface IEditorJsonValue
    {
        #region Properties.

        string Text { get; set; }
        string JsonDefaultValue { get; set; }
        string JsonValue { get; set; }
        IEditorJsonValueChangedCallback EditorJsonValueChangedCallback { get; set; }
        
        #endregion



        #region Functionality.

        void UpdateEditorValue();

        #endregion
    }
}
