using System;

namespace ZasuvkaPtakopyskaExtender.Visualizers
{
    [PtakopyskComponentVisualizer("SpriteRenderer")]
    public class SpriteRendererComponentVisualizer : IComponentVisualizer
    {
        private SFML.Graphics.RectangleShape m_sprite;

        public void OnInitialize(IVisualizerParent parent, SceneModel.GameObject gameObject)
        {
            m_sprite = new SFML.Graphics.RectangleShape();
        }

        public void OnRelease(IVisualizerParent parent, SceneModel.GameObject gameObject)
        {
            m_sprite.Dispose();
            m_sprite = null;
        }

        public void OnRender(IVisualizerParent parent, SFML.Graphics.RenderTarget target, SceneModel.GameObject gameObject)
        {
            SFML.Window.Vector2f pos = new SFML.Window.Vector2f();
            float rot = 0.0f;
            SFML.Window.Vector2f scl = new SFML.Window.Vector2f(1, 1);
            SFML.Graphics.Texture tex = null;
            SFML.Window.Vector2f siz = new SFML.Window.Vector2f();
            SFML.Window.Vector2f org = new SFML.Window.Vector2f();
            SFML.Graphics.Color col = SFML.Graphics.Color.White;
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
                    transform.GetPropertyValueOrDefault<float>("Rotation", out rot);
                    float[] scale = null;
                    if (transform.GetPropertyValueOrDefault<float[]>("Scale", out scale))
                    {
                        if (scale != null && scale.Length >= 2)
                            scl = new SFML.Window.Vector2f(scale[0], scale[1]);
                    }
                }
                SceneModel.GameObject.Component spriteRenderer = gameObject.components.Find(item => item.type == "SpriteRenderer");
                if (spriteRenderer != null)
                {
                    string texture = null;
                    if (spriteRenderer.GetPropertyValueOrDefault<string>("Texture", out texture))
                        tex = parent.FindTextureAsset(texture);
                    float[] size = null;
                    if (spriteRenderer.GetPropertyValueOrDefault<float[]>("Size", out size))
                    {
                        if (size != null && size.Length >= 2)
                            siz = new SFML.Window.Vector2f(size[0], size[1]);
                    }
                    float[] origin = null;
                    if (spriteRenderer.GetPropertyValueOrDefault<float[]>("Origin", out origin))
                    {
                        if (origin != null && origin.Length >= 2)
                            org = new SFML.Window.Vector2f(origin[0], origin[1]);
                    }
                    byte[] color = null;
                    if (spriteRenderer.GetPropertyValueOrDefault<byte[]>("Color", out color))
                    {
                        if (color != null && color.Length >= 4)
                            col = new SFML.Graphics.Color(color[0], color[1], color[2], color[3]);
                    }
                }
            }

            m_sprite.Position = pos;
            m_sprite.Rotation = rot;
            m_sprite.Scale = scl;
            m_sprite.Texture = tex;
            m_sprite.Size = siz;
            m_sprite.Origin = org;
            m_sprite.FillColor = col;
            target.Draw(m_sprite);
        }
    }
}
