namespace ZasuvkaPtakopyskaExtender.Editors
{
    public interface IEditorJsonValueChangedCallback
    {
        #region Functionality.

        void OnEditorValueChanged(IEditorJsonValue editor, string property, string jsonValue);

        #endregion
    }
}
