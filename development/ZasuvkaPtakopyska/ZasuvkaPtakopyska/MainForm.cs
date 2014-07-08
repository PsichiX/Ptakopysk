using System;
using System.Windows.Forms;
using MetroFramework.Forms;
using MetroFramework.Controls;
using System.Drawing;
using System.IO;
using System.Collections.Generic;
using Newtonsoft.Json;

namespace ZasuvkaPtakopyska
{
    public partial class MainForm : MetroForm
    {
        #region Public Nested Classes.
        
        public class Action
        {
            public string Id;
            public object[] Params;

            public Action(string id, params object[] args)
            {
                Id = id;
                Params = args;
            }
        }
        
        #endregion



        #region Private Static Data.

        private static readonly string DEFAULT_APP_TITLE = "Zásuvka Ptakopyska";
        
        #endregion



        #region Private Data.

        private MetroPanel m_mainPanel;
        private MetroTabControl m_mainPanelTabs;
        private WebBrowser m_welcomePage;
        private ProjectPageControl m_projectPage;
        private BuildPageControl m_buildPage;
        private ScenePageControl m_scenePage;
        private SettingsPageControl m_settingsPage;
        private MetroSidePanel m_leftPanel;
        private ProjectManagerControl m_projectManagerPanel;
        private MetroSidePanel m_rightPanel;
        private MetroSidePanel m_bottomPanel;

        private string m_appTitleExtended;
        private FileSystemWatcher m_fileSystemWatcher;
        private volatile bool m_isActive = false;
        private List<Action> m_actionsQueue = new List<Action>();
        private bool m_settingsValidated = false;

        #endregion



        #region Public Static Data.

        public static readonly string TAB_NAME_WELCOME = "Welcome Page";
        public static readonly string TAB_NAME_PROJECT = "Project";
        public static readonly string TAB_NAME_BUILD = "Build && Run";
        public static readonly string TAB_NAME_SCENE = "Scene";
        public static readonly string TAB_NAME_EDIT = "Edit";
        public static readonly string TAB_NAME_SETTINGS = "Settings";
        
        #endregion



        #region Public Properties.

        public string AppTitleExtended { get { return m_appTitleExtended; } set { m_appTitleExtended = value; RefreshAppTitle(); } }
        public ProjectModel ProjectModel { get { return m_projectPage == null ? null : m_projectPage.ProjectModel; } }
        public SettingsModel SettingsModel { get { return m_settingsPage == null ? null : m_settingsPage.SettingsModel; } }
        
        #endregion



        #region Construction and Destruction.

        public MainForm()
        {
            MetroSkinManager.SetManagerOwner(this);
            MetroSkinManager.ApplyMetroStyle(this);
            Load += new EventHandler(MainForm_Load);
            FormClosing += new FormClosingEventHandler(MainForm_FormClosing);
            Activated += new EventHandler(MainForm_Activated);
            Deactivate += new EventHandler(MainForm_Deactivate);
            Padding = new Padding(1, 0, 1, 20);
            Size = new Size(800, 600);
            RefreshAppTitle();

            m_fileSystemWatcher = new FileSystemWatcher();
            m_fileSystemWatcher.NotifyFilter = NotifyFilters.LastWrite | NotifyFilters.FileName | NotifyFilters.DirectoryName;
            m_fileSystemWatcher.Changed += new FileSystemEventHandler(m_fileSystemWatcher_Changed);
            m_fileSystemWatcher.Created += new FileSystemEventHandler(m_fileSystemWatcher_Created);
            m_fileSystemWatcher.Deleted += new FileSystemEventHandler(m_fileSystemWatcher_Deleted);
            m_fileSystemWatcher.Renamed += new RenamedEventHandler(m_fileSystemWatcher_Renamed);

            InitializeMainPanel();
        }

        #endregion



        #region Public Functionality.

        public void RefreshAppTitle()
        {
            Text = DEFAULT_APP_TITLE + (String.IsNullOrEmpty(m_appTitleExtended) ? "" : (" - " + m_appTitleExtended));
            Invalidate();
        }

        public TabPage AddTabPage(Control page, string name, bool select = false)
        {
            TabPage tab = new TabPage(name);
            tab.Controls.Add(page);
            m_mainPanelTabs.TabPages.Add(tab);
            if (select)
                m_mainPanelTabs.SelectedTab = tab;
            return tab;
        }

        public void RemoveTabPage(string name)
        {
            TabPage page;
            for (int i = m_mainPanelTabs.TabPages.Count - 1; i >= 0; --i)
            {
                page = m_mainPanelTabs.TabPages[i];
                if (page.Text == name)
                    m_mainPanelTabs.TabPages.Remove(page);
            }
        }

        public void SelectTabPage(string name)
        {
            foreach (TabPage page in m_mainPanelTabs.TabPages)
            {
                if (page.Text == name)
                {
                    m_mainPanelTabs.SelectedTab = page;
                    return;
                }
            }
        }

        public void WatchProjectFileSystem()
        {
            if (ProjectModel == null)
                return;

            m_fileSystemWatcher.Path = ProjectModel.WorkingDirectory;
            m_fileSystemWatcher.EnableRaisingEvents = true;
        }

        public void UnwatchProjectFileSystem()
        {
            m_fileSystemWatcher.EnableRaisingEvents = false; ;
        }

        public void InitializeGameEditorPages()
        {
            WatchProjectFileSystem();

            InitializeScenePage();
            InitializeBuildPage();
            //InitializeEditPage();
        }

        public void DeinitializeGameEditorPages()
        {
            UnwatchProjectFileSystem();
            
            if (m_scenePage != null)
            {
                RemoveTabPage(TAB_NAME_SCENE);
                m_scenePage.Dispose();
                m_scenePage = null;
            }
            if (m_buildPage != null)
            {
                RemoveTabPage(TAB_NAME_BUILD);
                m_buildPage.Dispose();
                m_buildPage = null;
            }
            if (m_projectManagerPanel != null)
                m_projectManagerPanel.RebuildList();
        }

        public bool DoAction(Action action, bool forcedUnique = false, bool forceExecute = false)
        {
            if (action == null)
                return false;

            if (m_isActive || forceExecute)
            {
                this.doOnUIThread(() => this.OnAction(action));
                return true;
            }
            else
            {
                EnqueueAction(action, forcedUnique);
                return false;
            }
        }

        public void EnqueueAction(Action action, bool forcedUnique = false)
        {
            lock (m_actionsQueue)
            {
                if (forcedUnique || !m_actionsQueue.Exists(item => item.Id == action.Id))
                    m_actionsQueue.Add(action);
            }
        }

        public void PerformPendingActions()
        {
            if (!m_isActive)
                return;

            lock (m_actionsQueue)
            {
                foreach (Action action in m_actionsQueue)
                    this.doOnUIThread(() => this.OnAction(action));
                m_actionsQueue.Clear();
            }
        }

        #endregion



        #region Private Functionality.

        private void InitializeMainPanel()
        {
            m_mainPanel = new MetroPanel();
            MetroSkinManager.ApplyMetroStyle(m_mainPanel);
            m_mainPanel.Dock = DockStyle.Fill;
            m_mainPanel.Padding = new Padding(20, 2, 20, 20);
            Controls.Add(m_mainPanel);

            // tabs.
            m_mainPanelTabs = new MetroTabControl();
            MetroSkinManager.ApplyMetroStyle(m_mainPanelTabs);
            m_mainPanelTabs.Left = m_mainPanel.Padding.Left;
            m_mainPanelTabs.Top = m_mainPanel.Padding.Top;
            m_mainPanelTabs.Width = m_mainPanel.Width - m_mainPanel.Padding.Horizontal;
            m_mainPanelTabs.Height = m_mainPanel.Height - m_mainPanel.Padding.Vertical;
            m_mainPanelTabs.Anchor = AnchorStyles.Left | AnchorStyles.Right | AnchorStyles.Top | AnchorStyles.Bottom;
            m_mainPanel.Controls.Add(m_mainPanelTabs);

            // initial pages.
            InitializeWelcomePage();
            InitializeSettingsPage();
            InitializeProjectPage();
            
            // sub-panels.
            InitializeLeftPanel();
            //InitializeRightPanel();
            //InitializeBottomPanel();
        }

        private void InitializeLeftPanel()
        {
            m_leftPanel = new MetroSidePanel();
            MetroSkinManager.ApplyMetroStyle(m_leftPanel);
            m_leftPanel.Text = "Project Manager";
            m_leftPanel.Side = DockStyle.Left;
            m_leftPanel.IsRolled = true;
            m_leftPanel.AnimatedRolling = false;
            m_leftPanel.OffsetPadding = new Padding(0, 38, 0, 24);
            m_leftPanel.Width = 250;
            m_leftPanel.Height = m_mainPanel.Height;
            m_leftPanel.Docked += new EventHandler(sidePanel_DockUndock);
            m_leftPanel.Undocked += new EventHandler(sidePanel_DockUndock);
            m_mainPanel.Controls.Add(m_leftPanel);
            m_leftPanel.BringToFront();
            
            m_projectManagerPanel = new ProjectManagerControl();
            m_projectManagerPanel.Dock = DockStyle.Fill;
            m_leftPanel.Content.Controls.Add(m_projectManagerPanel);
        }

        private void InitializeRightPanel()
        {
            m_rightPanel = new MetroSidePanel();
            MetroSkinManager.ApplyMetroStyle(m_rightPanel);
            m_rightPanel.Text = "Properties Explorer";
            m_rightPanel.Side = DockStyle.Right;
            m_rightPanel.IsRolled = true;
            m_rightPanel.AnimatedRolling = false;
            m_rightPanel.OffsetPadding = new Padding(0, 38, 0, 24);
            m_rightPanel.Width = 250;
            m_rightPanel.Height = m_mainPanel.Height;
            m_mainPanel.Controls.Add(m_rightPanel);
            m_rightPanel.BringToFront();
        }

        private void InitializeBottomPanel()
        {
            m_bottomPanel = new MetroSidePanel();
            MetroSkinManager.ApplyMetroStyle(m_bottomPanel);
            m_bottomPanel.Text = "Custom Actions";
            m_bottomPanel.Side = DockStyle.Bottom;
            m_bottomPanel.IsRolled = true;
            m_bottomPanel.AnimatedRolling = false;
            m_bottomPanel.Height = 250;
            m_bottomPanel.Width = m_mainPanel.Width;
            m_mainPanel.Controls.Add(m_bottomPanel);
            m_bottomPanel.BringToFront();
        }

        private void InitializeWelcomePage()
        {
            m_welcomePage = new WebBrowser();
            m_welcomePage.Dock = DockStyle.Fill;
            m_welcomePage.Navigate(new Uri("http://psichix.github.io/Ptakopysk/"));

            AddTabPage(m_welcomePage, TAB_NAME_WELCOME);
        }

        private void InitializeProjectPage()
        {
            m_projectPage = new ProjectPageControl();
            m_projectPage.Dock = DockStyle.Fill;
            m_mainPanel.Controls.Add(m_projectPage);

            AddTabPage(m_projectPage, TAB_NAME_PROJECT, true);
        }

        private void InitializeScenePage()
        {
            m_scenePage = new ScenePageControl();
            m_scenePage.Dock = DockStyle.Fill;
            m_mainPanel.Controls.Add(m_scenePage);

            AddTabPage(m_scenePage, TAB_NAME_SCENE);
        }

        private void InitializeBuildPage()
        {
            m_buildPage = new BuildPageControl();
            m_buildPage.Dock = DockStyle.Fill;
            m_mainPanel.Controls.Add(m_buildPage);

            AddTabPage(m_buildPage, TAB_NAME_BUILD);

            m_buildPage.RefreshContent();
            if (m_projectManagerPanel != null)
                m_projectManagerPanel.RebuildList();
        }

        private void InitializeEditPage()
        {
            MetroPanel page = new MetroPanel();
            MetroSkinManager.ApplyMetroStyle(page);
            page.Dock = DockStyle.Fill;
            m_mainPanel.Controls.Add(page);

            AddTabPage(page, TAB_NAME_EDIT);
        }

        private void InitializeSettingsPage()
        {
            m_settingsPage = new SettingsPageControl();
            m_settingsPage.Dock = DockStyle.Fill;
            m_mainPanel.Controls.Add(m_settingsPage);

            AddTabPage(m_settingsPage, TAB_NAME_SETTINGS);

            m_settingsPage.RefreshContent();
        }

        private bool ValidateSettings()
        {
            m_settingsValidated = false;
            if (SettingsModel == null)
            {
                MetroFramework.MetroMessageBox.Show(this, "Settings are not loaded!", "Settings Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return false;
            }

            if (!SettingsModel.ValidateSdkPath())
            {
                DialogResult result = MetroFramework.MetroMessageBox.Show(this, "Make sure that SDK Location is correctly pointing at SDK directory!\nDo you want to download Ptakopysk SDK?", "Invalid Settings", MessageBoxButtons.YesNo, MessageBoxIcon.Error);
                if (result == DialogResult.Yes)
                {
                    string json = File.ReadAllText("resources/settings/InstallationSettings.json");
                    InstallationSettings install = JsonConvert.DeserializeObject<InstallationSettings>(json);
                    if (m_welcomePage != null && install != null)
                    {
                        m_welcomePage.Navigate(install.PtakopyskDownloadUri);
                        SelectTabPage(TAB_NAME_WELCOME);
                    }
                    else
                        SelectTabPage(TAB_NAME_SETTINGS);
                }
                else
                    SelectTabPage(TAB_NAME_SETTINGS);
                return false;
            }
            if (!SettingsModel.ValidateCodeBlocksIdePath())
            {
                DialogResult result = MetroFramework.MetroMessageBox.Show(this, "Make sure that Code::Blocks IDE with MinGW Location is correctly pointing at Code::Blocks IDE with MinGW directory!\nDo you want to download Code::Blocks IDE?", "Invalid Settings", MessageBoxButtons.YesNo, MessageBoxIcon.Error);
                if (result == DialogResult.Yes)
                {
                    string json = File.ReadAllText("resources/settings/InstallationSettings.json");
                    InstallationSettings install = JsonConvert.DeserializeObject<InstallationSettings>(json);
                    if (m_welcomePage != null && install != null)
                    {
                        m_welcomePage.Navigate(install.CodeBlocksDownloadUri);
                        SelectTabPage(TAB_NAME_WELCOME);
                    }
                    else
                        SelectTabPage(TAB_NAME_SETTINGS);
                }
                else
                    SelectTabPage(TAB_NAME_SETTINGS);
                return false;
            }
            if (!SettingsModel.ValidateBashBinPath())
            {
                DialogResult result = MetroFramework.MetroMessageBox.Show(this, "Make sure that Bash Executable Location is correctly pointing at Bash Executable file!\nDo you want to download Msys GIT with Bash?", "Invalid Settings", MessageBoxButtons.YesNo, MessageBoxIcon.Error);
                if (result == DialogResult.Yes)
                {
                    string json = File.ReadAllText("resources/settings/InstallationSettings.json");
                    InstallationSettings install = JsonConvert.DeserializeObject<InstallationSettings>(json);
                    if (m_welcomePage != null && install != null)
                    {
                        m_welcomePage.Navigate(install.MsysGitBashDownloadUri);
                        SelectTabPage(TAB_NAME_WELCOME);
                    }
                    else
                        SelectTabPage(TAB_NAME_SETTINGS);
                }
                else
                    SelectTabPage(TAB_NAME_SETTINGS);
                return false;
            }
            m_settingsValidated = true;
            return true;
        }

        private void UpdateTabsLayout()
        {
            int left = m_leftPanel != null && m_leftPanel.IsDocked ? m_leftPanel.Width - MetroSidePanel.ROLLED_PART_SIZE : 0;
            int right = m_rightPanel != null && m_rightPanel.IsDocked ? m_rightPanel.Width - MetroSidePanel.ROLLED_PART_SIZE : 0;
            int bottom = m_bottomPanel != null && m_bottomPanel.IsDocked ? m_bottomPanel.Height - MetroSidePanel.ROLLED_PART_SIZE : 0;
            m_mainPanelTabs.Left = m_mainPanel.Padding.Left + left;
            m_mainPanelTabs.Top = m_mainPanel.Padding.Top;
            m_mainPanelTabs.Width = m_mainPanel.Width - m_mainPanel.Padding.Horizontal - left - right;
            m_mainPanelTabs.Height = m_mainPanel.Height - m_mainPanel.Padding.Vertical - bottom;
        }

        private void OnAction(Action action)
        {
            if (action == null)
                return;

            if (action.Id == "CbpChanged")
            {
                Console.WriteLine(">>> CBP project file changed");
                if (ProjectModel != null)
                {
                    ProjectModel.UpdateFromCbp();
                    if (m_buildPage != null)
                        m_buildPage.RefreshContent();
                    if (m_projectManagerPanel != null)
                        m_projectManagerPanel.RebuildList();
                }
            }
        }

        #endregion



        #region Private Events Handlers.

        private void MainForm_Load(object sender, EventArgs e)
        {
            MetroSkinManager.RefreshStyles();

            if (SettingsModel != null)
            {
                WindowState = SettingsModel.WindowState;
                if (m_leftPanel != null)
                {
                    m_leftPanel.IsRolled = SettingsModel.LeftPanelRolled;
                    m_leftPanel.IsDocked = SettingsModel.LeftPanelDocked;
                }
            }

            ValidateSettings();

            if (m_mainPanelTabs != null)
                m_mainPanelTabs.Selected += new TabControlEventHandler(m_mainPanelTabs_Selected);
        }

        private void MainForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (SettingsModel != null)
            {
                SettingsModel.WindowState = WindowState;
                if (m_leftPanel != null)
                {
                    SettingsModel.LeftPanelRolled = m_leftPanel.IsRolled;
                    SettingsModel.LeftPanelDocked = m_leftPanel.IsDocked;
                }
            }
        }

        private void MainForm_Deactivate(object sender, EventArgs e)
        {
            m_isActive = false;
        }

        private void MainForm_Activated(object sender, EventArgs e)
        {
            m_isActive = true;
            PerformPendingActions();
        }

        private void m_mainPanelTabs_Selected(object sender, TabControlEventArgs e)
        {
            if (!m_settingsValidated && e.Action == TabControlAction.Selected && e.TabPage.Text == TAB_NAME_PROJECT)
            {
                bool lastValid = m_settingsValidated;
                bool valid = ValidateSettings();
                if (!lastValid && valid && m_settingsPage != null)
                    m_settingsPage.SaveSettingsModel();
            }
        }

        private void m_fileSystemWatcher_Changed(object sender, FileSystemEventArgs e)
        {
            if (ProjectModel != null)
            {
                if (e.FullPath == ProjectModel.WorkingDirectory + @"\" + ProjectModel.CbpPath)
                    DoAction(new Action("CbpChanged"));
            }
        }

        private void m_fileSystemWatcher_Created(object sender, FileSystemEventArgs e)
        {
        }

        private void m_fileSystemWatcher_Deleted(object sender, FileSystemEventArgs e)
        {
        }

        private void m_fileSystemWatcher_Renamed(object sender, RenamedEventArgs e)
        {
        }

        private void sidePanel_DockUndock(object sender, EventArgs e)
        {
            UpdateTabsLayout();
        }

        #endregion
    }
}
