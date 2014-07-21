using System.Windows.Forms;
using MetroFramework.Controls;
using MetroFramework.Components;
using ZasuvkaPtakopyskaExtender;
using System.IO;

namespace ZasuvkaPtakopyska
{
    public partial class ScenePageControl : MetroUserControl
    {
        #region private Static Data.

        private static readonly int DEFAULT_TOOLBAR_HEIGHT = 48;
        
        #endregion



        #region Private Data.

        private MetroPanel m_toolbar;
        private RendererSurfaceControl m_renderer;
        private MetroSidePanel m_gameObjectsPanel;
        private MetroPanel m_gameObjectsList;
        private SceneModel m_model;
        private string m_scenePath;

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
        
        public bool OpenScene(string path)
        {
            CloseScene();
            if (!File.Exists(path))
                return false;

            string json = File.ReadAllText(path);
            m_model = Newtonsoft.Json.JsonConvert.DeserializeObject<SceneModel>(json);
            if (m_model == null)
                return false;
            m_model.PostProcessScene();
            RebuildSceneList();
            m_scenePath = path;
            return true;
        }

        public void CloseScene()
        {
            m_model = null;
            m_scenePath = null;
            RebuildSceneList();
        }
        
        #endregion



        #region Private Functionality.

        private void InitializeToolbar()
        {
            m_toolbar = new MetroPanel();
            MetroSkinManager.ApplyMetroStyle(m_toolbar);
            m_toolbar.Height = DEFAULT_TOOLBAR_HEIGHT;
            m_toolbar.Dock = DockStyle.Top;
            Controls.Add(m_toolbar);
        }

        private void InitializeSceneView()
        {
            m_renderer = new RendererSurfaceControl();
            m_renderer.Dock = DockStyle.Fill;
            Controls.Add(m_renderer);

            m_gameObjectsPanel = new MetroSidePanel();
            MetroSkinManager.ApplyMetroStyle(m_gameObjectsPanel);
            m_gameObjectsPanel.Text = "Game Objects";
            m_gameObjectsPanel.Side = DockStyle.Left;
            m_gameObjectsPanel.IsRolled = false;
            m_gameObjectsPanel.AnimatedRolling = false;
            m_gameObjectsPanel.IsDockable = false;
            m_gameObjectsPanel.OffsetPadding = new Padding(0, DEFAULT_TOOLBAR_HEIGHT, 0, 0);
            m_gameObjectsPanel.Width = 200;
            m_gameObjectsPanel.Height = Height;
            Controls.Add(m_gameObjectsPanel);
            m_gameObjectsPanel.BringToFront();

            m_gameObjectsList = new MetroPanel();
            MetroSkinManager.ApplyMetroStyle(m_gameObjectsList);
            m_gameObjectsList.Dock = DockStyle.Fill;
            m_gameObjectsPanel.Content.Controls.Add(m_gameObjectsList);
        }

        private void RebuildSceneList()
        {
            m_gameObjectsList.Controls.Clear();

            if (m_model == null || m_model.scene == null || m_model.scene.Count == 0)
                return;
            int y = 0;
            MetroButton btn;
            foreach (SceneModel.GameObject go in m_model.scene)
            {
                btn = new MetroButton();
                MetroSkinManager.ApplyMetroStyle(btn);
                btn.Tag = go;
                btn.Top = y;
                btn.Width = m_gameObjectsList.Width;
                if (go.properties != null)
                    btn.Text = go.properties.Id;
                btn.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
                btn.Click += new System.EventHandler(btn_Click);
                m_gameObjectsList.Controls.Add(btn);
                y = btn.Bottom;
            }
        }

        #endregion



        #region Private Events Handlers.

        private void btn_Click(object sender, System.EventArgs e)
        {
            MetroButton btn = sender as MetroButton;
            MainForm mainForm = FindForm() as MainForm;
            if (btn != null || mainForm != null || !(btn.Tag is SceneModel.GameObject))
                mainForm.ExploreGameObjectProperties(btn.Tag as SceneModel.GameObject);
        }

        #endregion
    }
}
