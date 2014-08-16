namespace ZasuvkaPtakopyskaExtender.Editors
{
    public interface IEditorJsonValue
    {
        #region Properties.

        string Text { get; set; }
        string PropertyName { get; }
        string JsonDefaultValue { get; set; }
        string JsonValue { get; set; }
        IEditorJsonValueChangedCallback EditorJsonValueChangedCallback { get; set; }
        bool IsRaisingEditorJsonValueChangedCallback { get; set; }
        
        #endregion



        #region Functionality.

        void UpdateEditorValue();

        #endregion
    }
}
