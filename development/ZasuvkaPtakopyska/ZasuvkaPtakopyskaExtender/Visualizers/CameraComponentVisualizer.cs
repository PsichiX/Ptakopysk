using System;

namespace ZasuvkaPtakopyskaExtender.Visualizers
{
    [PtakopyskComponentVisualizer("Camera")]
    public class CameraComponentVisualizer : IComponentVisualizer
    {
        private SFML.Graphics.RectangleShape m_frame;
        private SFML.Graphics.Vertex[] m_lines = new SFML.Graphics.Vertex[4];
        private SFML.Graphics.Text m_text;

        public void OnInitialize(IVisualizerParent parent, SceneModel.GameObject gameObject)
        {
            SFML.Graphics.Font font = parent.DefaultFontAsset;
            if (font == null)
                return;

            m_frame = new SFML.Graphics.RectangleShape();
            m_frame.FillColor = new SFML.Graphics.Color(0, 0, 0, 32);
            m_frame.OutlineColor = new SFML.Graphics.Color(255, 255, 255, 64);
            m_frame.OutlineThickness = 2;

            m_lines[0] = new SFML.Graphics.Vertex(new SFML.Window.Vector2f(), m_frame.OutlineColor);
            m_lines[1] = new SFML.Graphics.Vertex(new SFML.Window.Vector2f(), m_frame.OutlineColor);
            m_lines[2] = new SFML.Graphics.Vertex(new SFML.Window.Vector2f(), m_frame.OutlineColor);
            m_lines[3] = new SFML.Graphics.Vertex(new SFML.Window.Vector2f(), m_frame.OutlineColor);

            string name = gameObject.properties != null ? gameObject.properties.Id : "";
            m_text = new SFML.Graphics.Text(name, font, 16);
            m_text.Color = new SFML.Graphics.Color(255, 255, 0, 192);
            m_text.Style = SFML.Graphics.Text.Styles.Bold;
            SFML.Graphics.FloatRect rect = m_text.GetLocalBounds();
            m_text.Origin = new SFML.Window.Vector2f(rect.Width * 0.5f, 0);
        }

        public void OnRelease(IVisualizerParent parent, SceneModel.GameObject gameObject)
        {
            m_frame.Dispose();
            m_frame = null;
            m_lines = null;
            m_text.Dispose();
            m_text = null;
        }

        public void OnRender(IVisualizerParent parent, SFML.Graphics.RenderTarget target, SceneModel.GameObject gameObject)
        {
            if (m_frame == null || m_lines == null || m_text == null)
                return;

            SFML.Window.Vector2f pos = new SFML.Window.Vector2f();
            SFML.Window.Vector2f siz = new SFML.Window.Vector2f();
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
                SceneModel.GameObject.Component camera = gameObject.components.Find(item => item.type == "Camera");
                if (camera != null)
                {
                    float[] size = null;
                    if (camera.GetPropertyValueOrDefault<float[]>("Size", out size))
                    {
                        if (size != null && size.Length >= 2)
                        {
                            siz = new SFML.Window.Vector2f(size[0], size[1]);
                            SFML.Graphics.View view = target.GetView();
                            if (view != null)
                            {
                                if (siz.X < 0.0f)
                                    siz.X = view.Size.X;
                                if (siz.Y < 0.0f)
                                    siz.Y = view.Size.Y;
                            }
                        }
                    }
                }
            }

            m_frame.Size = siz;
            m_frame.Origin = new SFML.Window.Vector2f(m_frame.Size.X * 0.5f, m_frame.Size.Y * 0.5f);
            m_frame.Position = pos;
            target.Draw(m_frame);

            SFML.Graphics.FloatRect rect = m_frame.GetGlobalBounds();
            m_lines[0].Position = new SFML.Window.Vector2f(rect.Left + m_frame.OutlineThickness, rect.Top + m_frame.OutlineThickness);
            m_lines[1].Position = new SFML.Window.Vector2f(rect.Left + rect.Width - m_frame.OutlineThickness, rect.Top + rect.Height - m_frame.OutlineThickness);
            m_lines[2].Position = new SFML.Window.Vector2f(rect.Left + m_frame.OutlineThickness, rect.Top + rect.Height - m_frame.OutlineThickness);
            m_lines[3].Position = new SFML.Window.Vector2f(rect.Left + rect.Width - m_frame.OutlineThickness, rect.Top + m_frame.OutlineThickness);
            target.Draw(m_lines, SFML.Graphics.PrimitiveType.Lines);

            m_text.Position = pos - new SFML.Window.Vector2f(0, m_frame.Size.Y * 0.5f);
            target.Draw(m_text);
        }
    }
}
