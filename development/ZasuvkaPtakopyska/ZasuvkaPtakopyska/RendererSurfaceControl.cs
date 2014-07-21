using System;
using System.Windows.Forms;
using System.Drawing;

namespace ZasuvkaPtakopyska
{
    public partial class RendererSurfaceControl : UserControl
    {
        #region Nested Classes.

        public class GameObject
        {
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

        #endregion



        #region Public Properties.

        public SceneModel SceneModel { get { return m_model; } set { m_model = value; RebuildScene(); } }
        public float Zoom { get; set; }

        #endregion



        #region Construction and Destruction.

        public RendererSurfaceControl()
        {
            Load += new EventHandler(RendererSurfaceControl_Load);
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

        public void RebuildScene()
        {

        }

        #endregion



        #region Protected Functionality.

        protected override void OnPaint(PaintEventArgs e)
        {
            if (m_renderer == null || m_camera == null)
                return;

            m_renderer.DispatchEvents();
            m_renderer.Clear(new SFML.Graphics.Color(64, 64, 64, 255));
            m_camera.Size = new SFML.Window.Vector2f(m_renderer.Size.X, m_renderer.Size.Y);
            m_camera.Zoom(Zoom);
            m_renderer.SetView(m_camera);

            // draw grid
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
