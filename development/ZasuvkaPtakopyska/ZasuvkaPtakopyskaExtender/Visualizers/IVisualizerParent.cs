namespace ZasuvkaPtakopyskaExtender.Visualizers
{
    public interface IVisualizerParent
    {
        SFML.Graphics.Texture DefaultTextureAsset { get; }
        SFML.Graphics.Shader DefaultShaderAsset { get; }
        SFML.Graphics.Font DefaultFontAsset { get; }

        SFML.Graphics.Texture FindTextureAsset(string id);
        SFML.Graphics.Shader FindShaderAsset(string id);
        SFML.Graphics.Font FindFontAsset(string id);
    }
}
