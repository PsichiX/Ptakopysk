using System.Windows.Forms;
using MetroFramework.Controls;
using MetroFramework.Components;

namespace ZasuvkaPtakopyska
{
    public partial class ScenePageControl : MetroUserControl
    {
        #region Private Data.

        private MetroPanel m_toolbar;
        private RendererSurfaceControl m_renderer;

        #endregion



        #region Constructuion & Destruction.

        public ScenePageControl()
        {
            MetroSkinManager.ApplyMetroStyle(this);

            InitializeSceneView();
            InitializeToolbar();
        }

        #endregion



        #region Public Functionality.

        public void InitializeToolbar()
        {
            m_toolbar = new MetroPanel();
            MetroSkinManager.ApplyMetroStyle(m_toolbar);
            m_toolbar.Height = 48;
            m_toolbar.Dock = DockStyle.Top;
            Controls.Add(m_toolbar);
        }

        public void InitializeSceneView()
        {
            m_renderer = new RendererSurfaceControl();
            m_renderer.Dock = DockStyle.Fill;
            Controls.Add(m_renderer);
        }

        #endregion
    }
}
