using System.Windows.Forms;
using MetroFramework.Controls;
using ZasuvkaPtakopyskaExtender;
using System.IO;
using System.Collections.Generic;
using System;
using System.Drawing;

namespace ZasuvkaPtakopyska
{
    public partial class ScenePageControl : MetroUserControl
    {
        #region private Static Data.

        private static readonly int DEFAULT_TOOLBAR_HEIGHT = 42;
        private static readonly Size DEFAULT_TOOLBAR_ICON_SIZE = new Size(32, 32);
        private static readonly int DEFAULT_TOOLBAR_SEPARATOR = 4;
        private static readonly string GAME_OBJECTS_MODE = "Game Objects";
        private static readonly string PREFABS_MODE = "Prefabs";
        private static readonly string DEFAULT_SCENE_FILTER = "Ptakopysk scene file (*.json)|*.json";

        #endregion



        #region Private Data.

        private MetroPanel m_toolbar;
        private MetroPanel m_toolbarContent;
        private MetroScrollBar m_toolbarContentScrollbarH;
        private MetroComboBox m_gameObjectsModeComboBox;
        private MetroButton m_gameObjectsMenuButton;
        private MetroButton m_sceneMenuButton;
        private MetroButton m_assetsMenuButton;
        private RendererSurfaceControl m_renderer;
        private MetroSidePanel m_gameObjectsPanel;
        private TreeView m_gameObjectsTree;
        private string m_scenePath;

        #endregion



        #region Public Properties.

        public bool IsGameObjectsPrefabsMode
        {
            get
            {
                return m_gameObjectsModeComboBox != null &&
                    m_gameObjectsModeComboBox.SelectedValue as string == PREFABS_MODE;
            }
            set
            {
                if (m_gameObjectsModeComboBox != null)
                    m_gameObjectsModeComboBox.SelectedValue = value ? PREFABS_MODE : GAME_OBJECTS_MODE;
            }
        }

        #endregion



        #region Constructuion & Destruction.

        public ScenePageControl()
        {
            MetroSkinManager.ApplyMetroStyle(this);

            InitializeToolbar();
            InitializeSceneView();
        }

        #endregion



        #region Public Functionality.

        public bool OpenScene(string path)
        {
            CloseScene();
            if (!File.Exists(path))
                return false;

            MainForm mainForm = FindForm() as MainForm;
            if (mainForm != null)
                PtakopyskInterface.Instance.SetAssetsFileSystemRoot(mainForm.ProjectModel.WorkingDirectory + @"\" + mainForm.ProjectModel.ActiveTargetWorkingDirectory);
            string json = File.ReadAllText(path);
            m_renderer.RebuildSceneView(json);
            m_scenePath = path;
            RebuildSceneTree();
            return true;
        }

        public bool SaveScene(string path)
        {
            if (string.IsNullOrEmpty(m_scenePath))
                return false;

            string json = PtakopyskInterface.Instance.ConvertSceneToJson();
            File.WriteAllText(path, json);
            if (File.Exists(path))
            {
                m_scenePath = path;
                return true;
            }
            return false;
        }

        public void CloseScene()
        {
            m_renderer.RebuildSceneView(null);
            m_scenePath = null;
            PtakopyskInterface.Instance.SetAssetsFileSystemRoot("");
            RebuildSceneTree();
        }

        public void ReloadScene()
        {
            string p = m_scenePath;
            CloseScene();
            if (!string.IsNullOrEmpty(p))
                OpenScene(p);
        }

        public void RefreshSceneView()
        {
            if (m_renderer != null)
                m_renderer.Invalidate();
        }

        public void SceneTreeChangeGameObjectId(int handle)
        {
            if (m_gameObjectsTree != null)
                UpdateGameObjectNode(m_gameObjectsTree.Nodes, handle);
        }

        #endregion



        #region Private Functionality.

        private void InitializeToolbar()
        {
            m_toolbar = new MetroPanel();
            MetroSkinManager.ApplyMetroStyle(m_toolbar);
            m_toolbar.Left = 0;
            m_toolbar.Top = 0;
            m_toolbar.Width = Width;
            m_toolbar.Height = DEFAULT_TOOLBAR_HEIGHT;
            m_toolbar.Anchor = AnchorStyles.Left | AnchorStyles.Right | AnchorStyles.Top;
            Controls.Add(m_toolbar);

            m_toolbarContent = new MetroPanel();
            MetroSkinManager.ApplyMetroStyle(m_toolbarContent);
            m_toolbarContent.Dock = DockStyle.Fill;
            m_toolbarContent.Controls.Clear();
            m_toolbarContent.Resize += new EventHandler(m_toolbarContent_Resize);
            m_toolbarContent.ControlAdded += new ControlEventHandler(m_toolbarContent_ControlAddedRemoved);
            m_toolbarContent.ControlRemoved += new ControlEventHandler(m_toolbarContent_ControlAddedRemoved);
            m_toolbar.Controls.Add(m_toolbarContent);

            m_toolbarContentScrollbarH = new MetroScrollBar(MetroScrollOrientation.Horizontal);
            MetroSkinManager.ApplyMetroStyle(m_toolbarContentScrollbarH);
            m_toolbarContentScrollbarH.Dock = DockStyle.Bottom;
            m_toolbarContentScrollbarH.Scroll += new ScrollEventHandler(m_toolbarContentScrollbarH_Scroll);
            m_toolbar.Controls.Add(m_toolbarContentScrollbarH);

            int x = 0;

            m_gameObjectsModeComboBox = new MetroComboBox();
            MetroSkinManager.ApplyMetroStyle(m_gameObjectsModeComboBox);
            m_gameObjectsModeComboBox.Top = DEFAULT_TOOLBAR_SEPARATOR;
            m_gameObjectsModeComboBox.Left = x;
            m_gameObjectsModeComboBox.BindingContext = new BindingContext();
            m_gameObjectsModeComboBox.DataSource = new string[] { GAME_OBJECTS_MODE, PREFABS_MODE };
            m_gameObjectsModeComboBox.SelectedValueChanged += new EventHandler(m_gameObjectsModeComboBox_SelectedValueChanged);
            m_toolbarContent.Controls.Add(m_gameObjectsModeComboBox);
            x = m_gameObjectsModeComboBox.Right + DEFAULT_TOOLBAR_SEPARATOR;

            m_gameObjectsMenuButton = new MetroButton();
            MetroSkinManager.ApplyMetroStyle(m_gameObjectsMenuButton);
            m_gameObjectsMenuButton.Top = DEFAULT_TOOLBAR_SEPARATOR;
            m_gameObjectsMenuButton.Text = "Game Objects";
            m_gameObjectsMenuButton.Left = x;
            m_gameObjectsMenuButton.Width = m_gameObjectsMenuButton.GetPreferredSize(new Size()).Width + 20;
            m_gameObjectsMenuButton.Click += new EventHandler(m_gameObjectsMenuButton_Click);
            m_toolbarContent.Controls.Add(m_gameObjectsMenuButton);
            x = m_gameObjectsMenuButton.Right + DEFAULT_TOOLBAR_SEPARATOR;

            m_sceneMenuButton = new MetroButton();
            MetroSkinManager.ApplyMetroStyle(m_sceneMenuButton);
            m_sceneMenuButton.Top = DEFAULT_TOOLBAR_SEPARATOR;
            m_sceneMenuButton.Text = "Scene";
            m_sceneMenuButton.Left = x;
            m_sceneMenuButton.Width = m_sceneMenuButton.GetPreferredSize(new Size()).Width + 20;
            m_sceneMenuButton.Click += new EventHandler(m_sceneMenuButton_Click);
            m_toolbarContent.Controls.Add(m_sceneMenuButton);
            x = m_sceneMenuButton.Right + DEFAULT_TOOLBAR_SEPARATOR;

            m_assetsMenuButton = new MetroButton();
            MetroSkinManager.ApplyMetroStyle(m_assetsMenuButton);
            m_assetsMenuButton.Top = DEFAULT_TOOLBAR_SEPARATOR;
            m_assetsMenuButton.Text = "Assets";
            m_assetsMenuButton.Left = x;
            m_assetsMenuButton.Width = m_assetsMenuButton.GetPreferredSize(new Size()).Width + 20;
            m_assetsMenuButton.Click += new EventHandler(m_assetsMenuButton_Click);
            m_toolbarContent.Controls.Add(m_assetsMenuButton);
            x = m_assetsMenuButton.Right + DEFAULT_TOOLBAR_SEPARATOR;
        }

        private void InitializeSceneView()
        {
            m_renderer = new RendererSurfaceControl();
            m_renderer.Left = 0;
            m_renderer.Top = m_toolbar == null ? 0 : m_toolbar.Bottom;
            m_renderer.Width = Width;
            m_renderer.Height = Height;
            m_renderer.Anchor = AnchorStyles.Left | AnchorStyles.Right | AnchorStyles.Top | AnchorStyles.Bottom;
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
            m_gameObjectsPanel.Rolled += new EventHandler(m_gameObjectsPanel_RollUnroll);
            m_gameObjectsPanel.Unrolled += new EventHandler(m_gameObjectsPanel_RollUnroll);
            Controls.Add(m_gameObjectsPanel);
            m_gameObjectsPanel.BringToFront();

            m_gameObjectsTree = new TreeView();
            MetroSkinManager.ExtendMetroStyle(m_gameObjectsTree);
            m_gameObjectsTree.BorderStyle = System.Windows.Forms.BorderStyle.None;
            m_gameObjectsTree.Dock = DockStyle.Fill;
            m_gameObjectsTree.NodeMouseClick += new TreeNodeMouseClickEventHandler(m_gameObjectsTree_NodeMouseClick);
            m_gameObjectsPanel.Content.Controls.Add(m_gameObjectsTree);

            UpdateLayout();
            UpdateToolbarScrollbars();
        }

        private void RebuildSceneTree()
        {
            m_gameObjectsTree.Nodes.Clear();
            BuildSceneTreeNodes(m_gameObjectsTree.Nodes);
            m_gameObjectsTree.ExpandAll();

            MainForm mainForm = FindForm() as MainForm;
            if (mainForm != null)
                mainForm.ExploreGameObjectProperties(0, false);
        }

        private void BuildSceneTreeNodes(TreeNodeCollection nodes)
        {
            bool prefabs = IsGameObjectsPrefabsMode;
            if (PtakopyskInterface.Instance.StartIterateGameObjects(prefabs))
            {
                while (PtakopyskInterface.Instance.CanIterateGameObjectsNext(prefabs))
                {
                    PtakopyskInterface.Instance.StartQueryIteratedGameObject();
                    int handle = PtakopyskInterface.Instance.QueriedGameObjectHandle();
                    Dictionary<string, string> info = PtakopyskInterface.Instance.QueryGameObject("{ \"get\": { \"properties\": [ \"Id\" ] } }");
                    string id;
                    if (info != null && info.ContainsKey("properties/Id"))
                        id = Newtonsoft.Json.JsonConvert.DeserializeObject<string>(info["properties/Id"]);
                    else
                        id = handle.ToString();
                    TreeNode node = nodes.Add(id);
                    node.Tag = handle;
                    BuildSceneTreeNodes(node.Nodes);
                    PtakopyskInterface.Instance.IterateGameObjectsNext(prefabs);
                }
                PtakopyskInterface.Instance.EndIterateGameObjects();
                PtakopyskInterface.Instance.EndQueryGameObject();
            }
        }

        private void UpdateGameObjectNode(TreeNodeCollection nodes, int handle)
        {
            if (nodes == null || handle == 0)
                return;
            foreach (TreeNode node in nodes)
            {
                if (node.Tag.Equals(handle))
                {
                    Dictionary<string, string> info = PtakopyskInterface.Instance.QueryGameObject(
                        handle,
                        IsGameObjectsPrefabsMode,
                        "{ \"get\": { \"properties\": [ \"Id\" ] } }"
                        );
                    string id;
                    if (info != null && info.ContainsKey("properties/Id"))
                        id = Newtonsoft.Json.JsonConvert.DeserializeObject<string>(info["properties/Id"]);
                    else
                        id = handle.ToString();
                    node.Text = id;
                }
                UpdateGameObjectNode(node.Nodes, handle);
            }
        }

        private void UpdateLayout()
        {
            int left = m_gameObjectsPanel != null && m_gameObjectsPanel.IsRolled ? MetroSidePanel.ROLLED_PART_SIZE : m_gameObjectsPanel.Width;
            m_renderer.Left = left;
            m_renderer.Width = Width - left;
        }

        private void UpdateToolbarScrollbars()
        {
            if (m_toolbarContentScrollbarH == null || m_toolbarContent == null)
                return;

            if (m_toolbarContent.Controls.Count == 0)
            {
                m_toolbarContentScrollbarH.Maximum = 1;
                m_toolbarContentScrollbarH.LargeChange = 1;
                m_toolbarContentScrollbarH.Visible = false;
            }
            else
            {
                Rectangle rect;
                m_toolbarContent.CalculateContentsRectangle(out rect);
                m_toolbarContentScrollbarH.Maximum = rect.Width;
                m_toolbarContentScrollbarH.LargeChange = m_toolbarContent.Width;
                m_toolbarContentScrollbarH.Visible = m_toolbarContentScrollbarH.Maximum > m_toolbarContentScrollbarH.LargeChange;
            }
            m_toolbarContent.HorizontalScroll.Maximum = m_toolbarContentScrollbarH.Maximum;
            m_toolbarContent.HorizontalScroll.LargeChange = m_toolbarContentScrollbarH.LargeChange;
        }

        private void AddNewGameObject(bool isPrefab, int parent, string prefabSource = "")
        {
            if (string.IsNullOrEmpty(m_scenePath))
                return;

            MetroPromptBox prompt = new MetroPromptBox();
            prompt.Title = "New Game Object";
            prompt.Message = "Enter new Game Object id:";
            DialogResult result = prompt.ShowDialog();
            if (result == DialogResult.OK && !string.IsNullOrEmpty(prompt.Value))
            {
                int handle = PtakopyskInterface.Instance.CreateGameObject(isPrefab, parent, prefabSource, prompt.Value);
                if (handle != 0)
                {
                    RebuildSceneTree();
                    RefreshSceneView();
                }
            }
        }

        private void RemoveGameObject(int handle, bool isPrefab)
        {
            if (string.IsNullOrEmpty(m_scenePath))
                return;

            if (PtakopyskInterface.Instance.DestroyGameObject(handle, isPrefab))
            {
                RebuildSceneTree();
                RefreshSceneView();
            }
        }

        private void RemoveAllGameObjects(bool isPrefab)
        {
            if (string.IsNullOrEmpty(m_scenePath))
                return;

            if (PtakopyskInterface.Instance.ClearSceneGameObjects(isPrefab))
            {
                RebuildSceneTree();
                RefreshSceneView();
            }
        }

        #endregion



        #region Private Events Handlers.

        private void m_toolbarContentScrollbarH_Scroll(object sender, ScrollEventArgs e)
        {
            m_toolbarContent.HorizontalScroll.Value = e.NewValue;
        }

        private void m_toolbarContent_Resize(object sender, EventArgs e)
        {
            UpdateToolbarScrollbars();
        }

        private void m_toolbarContent_ControlAddedRemoved(object sender, ControlEventArgs e)
        {
            UpdateToolbarScrollbars();
        }

        private void m_gameObjectsModeComboBox_SelectedValueChanged(object sender, EventArgs e)
        {
            if (m_gameObjectsTree != null)
                RebuildSceneTree();
        }

        private void m_gameObjectsMenuButton_Click(object sender, EventArgs e)
        {
            if (string.IsNullOrEmpty(m_scenePath))
                return;

            MetroButton btn = sender as MetroButton;
            if (btn == null)
                return;

            MetroContextMenu menu = new MetroContextMenu(null);
            MetroSkinManager.ApplyMetroStyle(menu);
            ToolStripMenuItem menuItem;

            menuItem = new ToolStripMenuItem("Add new Game Object");
            menuItem.Click += new EventHandler(menuItem_gameObjectAdd_Click);
            menu.Items.Add(menuItem);

            menuItem = new ToolStripMenuItem("Remove all Game Objects");
            menuItem.Click += new EventHandler(menuItem_gameObjectsRemove_Click);
            menu.Items.Add(menuItem);

            menu.Show(btn, new Point(0, btn.Height));
        }

        private void m_sceneMenuButton_Click(object sender, EventArgs e)
        {
            if (string.IsNullOrEmpty(m_scenePath))
                return;

            MetroButton btn = sender as MetroButton;
            if (btn == null)
                return;

            MetroContextMenu menu = new MetroContextMenu(null);
            MetroSkinManager.ApplyMetroStyle(menu);
            ToolStripMenuItem menuItem;

            menuItem = new ToolStripMenuItem("Save Scene");
            menuItem.Click += new EventHandler(menuItem_saveScene_Click);
            menu.Items.Add(menuItem);

            menuItem = new ToolStripMenuItem("Save Scene As...");
            menuItem.Click += new EventHandler(menuItem_saveSceneAs_Click);
            menu.Items.Add(menuItem);

            menuItem = new ToolStripMenuItem("Reload Scene");
            menuItem.Click += new EventHandler(menuItem_reloadScene_Click);
            menu.Items.Add(menuItem);

            menuItem = new ToolStripMenuItem("Close Scene");
            menuItem.Click += new EventHandler(menuItem_closeScene_Click);
            menu.Items.Add(menuItem);

            menu.Show(btn, new Point(0, btn.Height));
        }

        private void m_assetsMenuButton_Click(object sender, EventArgs e)
        {
            if (string.IsNullOrEmpty(m_scenePath))
                return;

            MetroButton btn = sender as MetroButton;
            if (btn == null)
                return;

            MetroContextMenu menu = new MetroContextMenu(null);
            MetroSkinManager.ApplyMetroStyle(menu);
            ToolStripMenuItem menuItem;

            menuItem = new ToolStripMenuItem("Textures");
            menuItem.Click += new EventHandler(menuItem_textures_Click);
            menu.Items.Add(menuItem);

            menuItem = new ToolStripMenuItem("Shaders");
            menuItem.Click += new EventHandler(menuItem_shaders_Click);
            menu.Items.Add(menuItem);

            menuItem = new ToolStripMenuItem("Sounds");
            menuItem.Click += new EventHandler(menuItem_sounds_Click);
            menu.Items.Add(menuItem);

            menuItem = new ToolStripMenuItem("Music");
            menuItem.Click += new EventHandler(menuItem_music_Click);
            menu.Items.Add(menuItem);

            menuItem = new ToolStripMenuItem("Fonts");
            menuItem.Click += new EventHandler(menuItem_fonts_Click);
            menu.Items.Add(menuItem);

            menu.Show(btn, new Point(0, btn.Height));
        }

        private void menuItem_gameObjectAdd_Click(object sender, EventArgs e)
        {
            AddNewGameObject(IsGameObjectsPrefabsMode, 0);
        }

        private void menuItem_gameObjectsRemove_Click(object sender, EventArgs e)
        {
            RemoveAllGameObjects(IsGameObjectsPrefabsMode);
        }

        private void menuItem_saveScene_Click(object sender, EventArgs e)
        {
            SaveScene(m_scenePath);
        }

        private void menuItem_saveSceneAs_Click(object sender, EventArgs e)
        {
            if (string.IsNullOrEmpty(m_scenePath))
                return;

            SaveFileDialog dialog = new SaveFileDialog();
            dialog.InitialDirectory = Path.GetDirectoryName(m_scenePath);
            dialog.FileName = Path.GetFileName(m_scenePath);
            dialog.RestoreDirectory = true;
            dialog.OverwritePrompt = true;
            dialog.Filter = DEFAULT_SCENE_FILTER;
            DialogResult result = dialog.ShowDialog();
            if (result == DialogResult.OK)
                SaveScene(dialog.FileName);
        }

        private void menuItem_reloadScene_Click(object sender, EventArgs e)
        {
            ReloadScene();
        }

        private void menuItem_closeScene_Click(object sender, EventArgs e)
        {
            CloseScene();
        }

        private void menuItem_textures_Click(object sender, EventArgs e)
        {
            MainForm mainForm = FindForm() as MainForm;
            if (mainForm != null)
                mainForm.ExploreAssetsProperties(PtakopyskInterface.AssetType.Texture);
        }

        private void menuItem_shaders_Click(object sender, EventArgs e)
        {
            MainForm mainForm = FindForm() as MainForm;
            if (mainForm != null)
                mainForm.ExploreAssetsProperties(PtakopyskInterface.AssetType.Shader);
        }

        private void menuItem_sounds_Click(object sender, EventArgs e)
        {
            MainForm mainForm = FindForm() as MainForm;
            if (mainForm != null)
                mainForm.ExploreAssetsProperties(PtakopyskInterface.AssetType.Sound);
        }

        private void menuItem_music_Click(object sender, EventArgs e)
        {
            MainForm mainForm = FindForm() as MainForm;
            if (mainForm != null)
                mainForm.ExploreAssetsProperties(PtakopyskInterface.AssetType.Music);
        }

        private void menuItem_fonts_Click(object sender, EventArgs e)
        {
            MainForm mainForm = FindForm() as MainForm;
            if (mainForm != null)
                mainForm.ExploreAssetsProperties(PtakopyskInterface.AssetType.Font);
        }

        private void m_gameObjectsPanel_RollUnroll(object sender, EventArgs e)
        {
            UpdateLayout();
        }

        private void m_gameObjectsTree_NodeMouseClick(object sender, TreeNodeMouseClickEventArgs e)
        {
            MainForm mainForm = FindForm() as MainForm;
            if (mainForm == null)
                return;

            TreeNode node = e.Node;
            if (e.Button == MouseButtons.Left)
            {
                if (node != null && node.Tag is int)
                    mainForm.ExploreGameObjectProperties((int)node.Tag, IsGameObjectsPrefabsMode);
                else
                    mainForm.ExploreGameObjectProperties(0, false);
            }
            else if (e.Button == MouseButtons.Right)
            {
                MetroContextMenu menu = new MetroContextMenu(null);
                MetroSkinManager.ApplyMetroStyle(menu);
                ToolStripMenuItem menuItem;

                menuItem = new ToolStripMenuItem("Remove");
                menuItem.Tag = node.Tag;
                menuItem.Click += new System.EventHandler(menuItem_nodeRemove_Click);
                menu.Items.Add(menuItem);

                menuItem = new ToolStripMenuItem("Add new Game Object");
                menuItem.Tag = node.Tag;
                menuItem.Click += new System.EventHandler(menuItem_nodeAdd_Click);
                menu.Items.Add(menuItem);

                if (IsGameObjectsPrefabsMode)
                {
                    menuItem = new ToolStripMenuItem("Instantiate on scene");
                    menuItem.Tag = node.Text;
                    menuItem.Click += new System.EventHandler(menuItem_nodeInstantiate_Click);
                    menu.Items.Add(menuItem);
                }

                menu.Show(m_gameObjectsTree, e.Location);
            }
        }

        private void menuItem_nodeRemove_Click(object sender, System.EventArgs e)
        {
            ToolStripMenuItem menuItem = sender as ToolStripMenuItem;
            if (menuItem == null || !(menuItem.Tag is int))
                return;

            RemoveGameObject((int)menuItem.Tag, IsGameObjectsPrefabsMode);
        }

        private void menuItem_nodeAdd_Click(object sender, System.EventArgs e)
        {
            ToolStripMenuItem menuItem = sender as ToolStripMenuItem;
            if (menuItem == null || !(menuItem.Tag is int))
                return;

            AddNewGameObject(IsGameObjectsPrefabsMode, (int)menuItem.Tag);
        }

        private void menuItem_nodeInstantiate_Click(object sender, System.EventArgs e)
        {
            ToolStripMenuItem menuItem = sender as ToolStripMenuItem;
            if (menuItem == null || !(menuItem.Tag is string))
                return;

            AddNewGameObject(false, 0, menuItem.Tag as string);
        }

        #endregion
    }
}
