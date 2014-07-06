using System;
using System.Windows.Forms;
using SFML.Graphics;

namespace ZasuvkaPtakopyska
{
    public partial class RendererSurfaceControl : UserControl
    {
        #region Private Data.

        private RenderWindow m_renderer;
        private Timer m_timer;

        #endregion



        #region Construction and Destruction.

        public RendererSurfaceControl()
        {
            Load += new EventHandler(RendererSurfaceControl_Load);

            m_timer = new Timer();
            m_timer.Interval = 1000 / 20;
            m_timer.Enabled = true;
            m_timer.Tick += new EventHandler(m_timer_Tick);
        }

        #endregion



        #region Protected Functionality.

        protected override void OnPaint(PaintEventArgs e)
        {
            if (m_renderer == null)
                return;

            m_renderer.DispatchEvents();
            m_renderer.Clear(new SFML.Graphics.Color(64, 64, 64, 255));
            m_renderer.Display();
        }

        protected override void OnPaintBackground(PaintEventArgs pevent)
        {
        }
        
        #endregion



        #region Private Events Handlers.

        private void RendererSurfaceControl_Load(object sender, EventArgs e)
        {
            m_renderer = new RenderWindow(Handle);
        }

        private void m_timer_Tick(object sender, EventArgs e)
        {
            Invalidate();
        }

        #endregion
    }
}
