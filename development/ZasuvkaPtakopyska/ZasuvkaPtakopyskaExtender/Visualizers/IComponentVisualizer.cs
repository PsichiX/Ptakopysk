namespace ZasuvkaPtakopyskaExtender.Visualizers
{
    public interface IComponentVisualizer
    {
        void OnInitialize(IVisualizerParent parent, SceneModel.GameObject gameObject);
        void OnRelease(IVisualizerParent parent, SceneModel.GameObject gameObject);
        void OnRender(IVisualizerParent parent, SFML.Graphics.RenderTarget target, SceneModel.GameObject gameObject);
    }
}
