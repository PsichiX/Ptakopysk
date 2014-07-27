using System;

namespace ZasuvkaPtakopyskaExtender.Visualizers
{
    public class DefaultComponentVisualizer : IComponentVisualizer
    {
        private SFML.Graphics.RectangleShape m_bg;
        private SFML.Graphics.Text m_text;
        private SFML.Graphics.RectangleShape m_icon;

        public void OnInitialize(IVisualizerParent parent, SceneModel.GameObject gameObject)
        {
            SFML.Graphics.Font font = parent.DefaultFontAsset;
            if (font == null)
                return;

            string name = gameObject.properties != null ? gameObject.properties.Id : "";
            m_text = new SFML.Graphics.Text(name, font, 16);
            m_text.Color = new SFML.Graphics.Color(255, 255, 0, 192);
            m_text.Style = SFML.Graphics.Text.Styles.Bold;
            SFML.Graphics.FloatRect rect = m_text.GetLocalBounds();
            m_text.Origin = new SFML.Window.Vector2f(rect.Width * 0.5f, 0);

            m_icon = new SFML.Graphics.RectangleShape(new SFML.Window.Vector2f(64, 64));
            m_icon.Origin = new SFML.Window.Vector2f(32, 64);
            m_icon.FillColor = new SFML.Graphics.Color(255, 255, 255, 64);
            m_icon.Texture = parent.DefaultTextureAsset;

            m_bg = new SFML.Graphics.RectangleShape(new SFML.Window.Vector2f(
                Math.Max(rect.Width, m_icon.Size.X) + 20,
                rect.Height + m_icon.Size.Y + 20
                ));
            m_bg.FillColor = new SFML.Graphics.Color(0, 0, 0, 128);
            m_bg.Origin = new SFML.Window.Vector2f(m_bg.Size.X * 0.5f, m_bg.Size.Y * 0.5f);
        }

        public void OnRelease(IVisualizerParent parent, SceneModel.GameObject gameObject)
        {
            m_bg.Dispose();
            m_bg = null;
            m_text.Dispose();
            m_text = null;
            m_icon.Dispose();
            m_icon = null;
        }

        public void OnRender(IVisualizerParent parent, SFML.Graphics.RenderTarget target, SceneModel.GameObject gameObject)
        {
            if (m_text == null)
                return;

            SFML.Window.Vector2f pos = new SFML.Window.Vector2f();
            if (gameObject.components != null)
            {
                SceneModel.GameObject.Component transform = gameObject.components.Find(item => item.type == "Transform");
                if (transform != null)
                {
                    float[] position = null;
                    if (transform.GetPropertyValueOrDefault<float[]>("Position", out position))
                    {
                        if (position != null && position.Length >= 2)
                            pos = new SFML.Window.Vector2f(position[0], position[1]);
                    }
                }
            }
            
            m_bg.Position = pos;
            target.Draw(m_bg);

            m_icon.Position = pos + new SFML.Window.Vector2f(0, m_bg.Size.Y * 0.5f);
            target.Draw(m_icon);

            m_text.Position = pos - new SFML.Window.Vector2f(0, m_bg.Size.Y * 0.5f);
            target.Draw(m_text);
        }
    }
}
