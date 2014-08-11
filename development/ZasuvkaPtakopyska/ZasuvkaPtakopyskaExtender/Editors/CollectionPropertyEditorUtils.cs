namespace ZasuvkaPtakopyskaExtender.Editors
{
    public static class CollectionPropertyEditorUtils
    {
        public enum CollectionType
        {
            JsonArray,
            JsonObject
        }

        public interface IValidator<T>
        {
            bool ValidateAdd(T item);
            bool ValidateRemove(T item);
        }
    }
}
