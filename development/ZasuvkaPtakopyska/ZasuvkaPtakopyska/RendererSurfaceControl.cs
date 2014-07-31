using System;
using System.Windows.Forms;
using System.Drawing;
using ZasuvkaPtakopyskaExtender;
using ZasuvkaPtakopyskaExtender.Visualizers;
using System.Collections.Generic;

namespace ZasuvkaPtakopyska
{
    public class RendererSurfaceControl : UserControl, IVisualizerParent
    {
        #region Nested Classes.

        public class GameObjectVisualizer
        {
            private SceneModel.GameObject m_model;
            private List<IComponentVisualizer> m_components = new List<IComponentVisualizer>();

            public SceneModel.GameObject Model { get { return m_model; } }

            public GameObjectVisualizer(SceneModel.GameObject model)
            {
                m_model = model;
            }

            public void Initialize(IVisualizerParent parent)
            {
                Release(parent);
                if (m_model == null)
                    return;

                foreach (SceneModel.GameObject.Component c in m_model.components)
                {
                    Type t = ComponentVisualizersManager.Instance.FindComponentVisualizerByComponentType(c.type);
                    if (t != null && !m_components.Exists(item => item.GetType() == t))
                    {
                        try
                        {
                            object obj = Activator.CreateInstance(t);
                            IComponentVisualizer visualizer = obj as IComponentVisualizer;
                            if (visualizer != null)
                            {
                                m_components.Add(visualizer);
                                visualizer.OnInitialize(parent, m_model);
                            }
                        }
                        catch { }
                    }
                }
                if (m_components.Count == 0)
                {
                    try
                    {
                        object obj = Activator.CreateInstance(typeof(DefaultComponentVisualizer));
                        IComponentVisualizer visualizer = obj as IComponentVisualizer;
                        if (visualizer != null)
                        {
                            m_components.Add(visualizer);
                            visualizer.OnInitialize(parent, m_model);
                        }
                    }
                    catch { }
                }
            }

            public void Release(IVisualizerParent parent)
            {
                foreach (IComponentVisualizer c in m_components)
                    c.OnRelease(parent, m_model);
                m_components.Clear();
            }

            public void Render(IVisualizerParent parent, SFML.Graphics.RenderTarget target)
            {
                foreach (IComponentVisualizer c in m_components)
                    c.OnRender(parent, target, m_model);
            }
        }

        #endregion



        #region Private Static Functionality.
        
        private static int CompareGameObjectsByOrder(GameObjectVisualizer a, GameObjectVisualizer b)
        {
            int oa = a == null || a.Model == null || a.Model.properties == null ? 0 : a.Model.properties.Order;
            int ob = b == null || b.Model == null || b.Model.properties == null ? 0 : b.Model.properties.Order;
            return ob.CompareTo(oa);
        }

        #endregion



        #region Private Static Data.

        private static readonly float DEFAULT_GRID_SIZE = 64.0f;

        #endregion



        #region Private Data.

        private SceneModel m_model;
        private SFML.Graphics.RenderWindow m_renderer;
        private Timer m_timer;
        private SFML.Graphics.View m_camera;
        private bool m_isDragging = false;
        private Point m_dragPosition;
        private Dictionary<string, SFML.Graphics.Texture> m_textures = new Dictionary<string, SFML.Graphics.Texture>();
        private Dictionary<string, SFML.Graphics.Shader> m_shaders = new Dictionary<string, SFML.Graphics.Shader>();
        private Dictionary<string, SFML.Graphics.Font> m_fonts = new Dictionary<string, SFML.Graphics.Font>();
        private List<GameObjectVisualizer> m_gameObjects = new List<GameObjectVisualizer>();
        private SFML.Graphics.Texture m_defaultTexture;
        private SFML.Graphics.Shader m_defaultShader;
        private SFML.Graphics.Font m_defaultFont;

        #endregion



        #region Public Properties.

        public SceneModel SceneModel { get { return m_model; } set { m_model = value; RebuildScene(); } }
        public float Zoom { get; set; }
        public SFML.Graphics.Texture DefaultTextureAsset { get { return m_defaultTexture; } }
        public SFML.Graphics.Shader DefaultShaderAsset { get { return m_defaultShader; } }
        public SFML.Graphics.Font DefaultFontAsset { get { return m_defaultFont; } }

        #endregion



        #region Construction and Destruction.

        public RendererSurfaceControl()
        {
            Load += new EventHandler(RendererSurfaceControl_Load);
            Disposed += new EventHandler(RendererSurfaceControl_Disposed);
            MouseDown += new MouseEventHandler(RendererSurfaceControl_MouseDown);
            MouseMove += new MouseEventHandler(RendererSurfaceControl_MouseMove);
            MouseUp += new MouseEventHandler(RendererSurfaceControl_MouseUp);

            Zoom = 1.0f;

            m_timer = new Timer();
            m_timer.Interval = 1000 / 20;
            m_timer.Enabled = false;
            m_timer.Tick += new EventHandler(m_timer_Tick);
        }

        #endregion



        #region Public Functionality.

        public SFML.Graphics.Texture FindTextureAsset(string id)
        {
            return id != null && m_textures.ContainsKey(id) ? m_textures[id] : null;
        }

        public SFML.Graphics.Shader FindShaderAsset(string id)
        {
            return id != null && m_shaders.ContainsKey(id) ? m_shaders[id] : null;
        }

        public SFML.Graphics.Font FindFontAsset(string id)
        {
            return id != null && m_fonts.ContainsKey(id) ? m_fonts[id] : null;
        }

        public void ClearGameObjects()
        {
            foreach (GameObjectVisualizer go in m_gameObjects)
                go.Release(this);
            m_gameObjects.Clear();
        }

        public void RebuildScene()
        {
            ClearGameObjects();
            if (m_model == null)
                return;

            foreach (SceneModel.GameObject go in m_model.scene)
            {
                GameObjectVisualizer visualizer = new GameObjectVisualizer(go);
                m_gameObjects.Add(visualizer);
                visualizer.Initialize(this);
            }
        }

        #endregion



        #region Protected Functionality.

        protected override void OnPaint(PaintEventArgs e)
        {
            if (m_renderer == null || m_camera == null || m_gameObjects == null)
                return;

            m_renderer.DispatchEvents();
            m_renderer.Clear(new SFML.Graphics.Color(64, 64, 64, 255));
            m_camera.Size = new SFML.Window.Vector2f(m_renderer.Size.X, m_renderer.Size.Y);
            m_camera.Zoom(Zoom);
            m_renderer.SetView(m_camera);

            // draw grid.
            float fleft = m_camera.Center.X - m_camera.Size.X * 0.5f;
            float ftop = m_camera.Center.Y - m_camera.Size.Y * 0.5f;
            float fright = m_camera.Center.X + m_camera.Size.X * 0.5f;
            float fbottom = m_camera.Center.Y + m_camera.Size.Y * 0.5f;
            int left = (int)(fleft / DEFAULT_GRID_SIZE);
            int top = (int)(ftop / DEFAULT_GRID_SIZE);
            int right = (int)(fright / DEFAULT_GRID_SIZE);
            int bottom = (int)(fbottom / DEFAULT_GRID_SIZE);
            SFML.Graphics.Vertex[] line = new SFML.Graphics.Vertex[2];
            SFML.Graphics.Color col = new SFML.Graphics.Color(255, 255, 255, 64);
            line[0].Color = col;
            line[1].Color = col;
            line[0].Position.Y = ftop;
            line[1].Position.Y = fbottom;
            for (int i = left; i <= right; ++i)
            {
                line[0].Position.X = i * DEFAULT_GRID_SIZE;
                line[1].Position.X = i * DEFAULT_GRID_SIZE;
                m_renderer.Draw(line, SFML.Graphics.PrimitiveType.Lines);
            }
            line[0].Position.X = fleft;
            line[1].Position.X = fright;
            for (int i = top; i <= bottom; ++i)
            {
                line[0].Position.Y = i * DEFAULT_GRID_SIZE;
                line[1].Position.Y = i * DEFAULT_GRID_SIZE;
                m_renderer.Draw(line, SFML.Graphics.PrimitiveType.Lines);
            }

            // sort game objects.
            m_gameObjects.Sort(CompareGameObjectsByOrder);
            
            // draw game objects.
            foreach (GameObjectVisualizer go in m_gameObjects)
                go.Render(this, m_renderer);

            // show rendered image.
            m_renderer.Display();
        }

        protected override void OnPaintBackground(PaintEventArgs pevent)
        {
        }

        #endregion



        #region Private Events Handlers.

        private void RendererSurfaceControl_Load(object sender, EventArgs e)
        {
            m_renderer = new SFML.Graphics.RenderWindow(Handle);
            m_renderer.SetVerticalSyncEnabled(false);
            m_camera = new SFML.Graphics.View();
            m_camera.Center = new SFML.Window.Vector2f();
            try { m_defaultTexture = new SFML.Graphics.Texture("resources/textures/logo.png"); }
            catch (Exception ex) { while (ex.InnerException != null)ex = ex.InnerException; Console.WriteLine("{0}: {1}", ex.GetType().Name, ex.Message); }
            try { m_defaultShader = new SFML.Graphics.Shader("resources/shaders/color-texture.vs", "resources/shaders/color-texture.fs"); }
            catch (Exception ex) { while (ex.InnerException != null)ex = ex.InnerException; Console.WriteLine("{0}: {1}", ex.GetType().Name, ex.Message); }
            try { m_defaultFont = new SFML.Graphics.Font("resources/fonts/verdana.ttf"); }
            catch (Exception ex) { while (ex.InnerException != null)ex = ex.InnerException; Console.WriteLine("{0}: {1}", ex.GetType().Name, ex.Message); }
        }

        private void RendererSurfaceControl_Disposed(object sender, EventArgs e)
        {
            if (m_defaultTexture != null)
            {
                m_defaultTexture.Dispose();
                m_defaultTexture = null;
            }
            if (m_defaultShader != null)
            {
                m_defaultShader.Dispose();
                m_defaultShader = null;
            }
            if (m_defaultFont != null)
            {
                m_defaultFont.Dispose();
                m_defaultFont = null;
            }
            if (m_camera != null)
            {
                m_camera.Dispose();
                m_camera = null;
            }
            if (m_renderer != null)
            {
                m_renderer.Dispose();
                m_renderer = null;
            }
            if (m_gameObjects != null)
            {
                m_gameObjects.Clear();
                m_gameObjects = null;
            }
            if (m_textures != null)
            {
                foreach (SFML.Graphics.Texture t in m_textures.Values)
                    t.Dispose();
                m_textures.Clear();
                m_textures = null;
            }
            if (m_shaders != null)
            {
                foreach (SFML.Graphics.Shader s in m_shaders.Values)
                    s.Dispose();
                m_shaders.Clear();
                m_shaders = null;
            }
            if (m_fonts != null)
            {
                foreach (SFML.Graphics.Font f in m_fonts.Values)
                    f.Dispose();
                m_fonts.Clear();
                m_fonts = null;
            }
        }

        private void m_timer_Tick(object sender, EventArgs e)
        {
            Invalidate();
        }

        private void RendererSurfaceControl_MouseDown(object sender, MouseEventArgs e)
        {
            if (e.Button != MouseButtons.Left)
                return;

            m_isDragging = true;
            m_dragPosition = e.Location;
        }

        private void RendererSurfaceControl_MouseMove(object sender, MouseEventArgs e)
        {
            if (!m_isDragging)
                return;

            float dx = (float)(m_dragPosition.X - e.X) * Zoom;
            float dy = (float)(m_dragPosition.Y - e.Y) * Zoom;
            m_dragPosition = e.Location;

            if (m_camera != null)
                m_camera.Move(new SFML.Window.Vector2f(dx, dy));

            Invalidate();
        }

        private void RendererSurfaceControl_MouseUp(object sender, MouseEventArgs e)
        {
            m_isDragging = false;
        }

        #endregion
    }
}
