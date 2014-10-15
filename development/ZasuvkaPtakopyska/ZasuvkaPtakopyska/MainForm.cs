using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Reflection;
using System.Threading.Tasks;
using System.Windows.Forms;
using MetroFramework.Controls;
using MetroFramework.Forms;
using Newtonsoft.Json;
using PtakopyskMetaGenerator;
using ZasuvkaPtakopyskaExtender;
using ZasuvkaPtakopyskaExtender.Editors;

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

        public class WorkingProcessOverlay : MetroUserControl
        {
            public WorkingProcessOverlay()
            {
                SetStyle(ControlStyles.SupportsTransparentBackColor, true);
                SetStyle(ControlStyles.Opaque, true);
            }

            protected override CreateParams CreateParams
            {
                get
                {
                    CreateParams parms = base.CreateParams;
                    parms.ExStyle |= 0x20;
                    return parms;
                }
            }

            protected override void OnPaintBackground(PaintEventArgs e)
            {
            }

            protected override void OnPaint(PaintEventArgs e)
            {
            }
        }

        #endregion



        #region Private Static Data.

        private static readonly string DEFAULT_APP_TITLE = "Zásuvka Ptakopyska";
        private static readonly string APP_CODE_BLOCKS = "Code::Blocks";
        private static readonly string APP_SCENE_EDITOR = "Scene Editor";
        private static readonly string APP_CONFIG_EDITOR = "Game Config Editor";

        #endregion



        #region Private Data.

        private WorkingProcessOverlay m_workingProcessOverlay;
        private MetroPanel m_mainPanel;
        private MetroTabControl m_mainPanelTabs;
        private WebBrowser m_welcomePage;
        private ProjectPageControl m_projectPage;
        private BuildPageControl m_buildPage;
        private ScenePageControl m_scenePage;
        private SettingsPageControl m_settingsPage;
        private MetroSidePanel m_leftPanel;
        private ProjectManagerControl m_projectManagerPanel;
        private ProjectFilesControl m_projectFilesPanel;
        private MetroSidePanel m_rightPanel;
        private MetroSidePanel m_bottomPanel;

        private string m_appTitleExtended;
        private FileSystemWatcher m_projectFileSystemWatcher;
        private FileSystemWatcher m_sdkFileSystemWatcher;
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
        public ProjectFilesControl ProjectFilesViewer { get { return m_projectFilesPanel; } }
        public bool IsWorkingProcessOverlayEnabled
        {
            get { return m_workingProcessOverlay != null ? m_workingProcessOverlay.Visible : false; }
            set
            {
                if (m_workingProcessOverlay != null)
                {
                    m_workingProcessOverlay.Visible = value;
                    if (value)
                        m_workingProcessOverlay.BringToFront();
                    else
                        m_workingProcessOverlay.SendToBack();
                }
            }
        }

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

            foreach (Assembly assembly in AppDomain.CurrentDomain.GetAssemblies())
                PropertyEditorsManager.Instance.RegisterPropertyEditorsFromAssembly(assembly);

            m_projectFileSystemWatcher = new FileSystemWatcher();
            m_projectFileSystemWatcher.IncludeSubdirectories = true;
            m_projectFileSystemWatcher.NotifyFilter = NotifyFilters.LastWrite | NotifyFilters.FileName | NotifyFilters.DirectoryName;
            m_projectFileSystemWatcher.Changed += new FileSystemEventHandler(m_fileSystemWatcher_Changed);
            m_projectFileSystemWatcher.Created += new FileSystemEventHandler(m_fileSystemWatcher_Created);
            m_projectFileSystemWatcher.Deleted += new FileSystemEventHandler(m_fileSystemWatcher_Deleted);
            m_projectFileSystemWatcher.Renamed += new RenamedEventHandler(m_fileSystemWatcher_Renamed);

            m_sdkFileSystemWatcher = new FileSystemWatcher();
            m_sdkFileSystemWatcher.IncludeSubdirectories = true;
            m_sdkFileSystemWatcher.NotifyFilter = NotifyFilters.LastWrite | NotifyFilters.FileName | NotifyFilters.DirectoryName;
            m_sdkFileSystemWatcher.Changed += new FileSystemEventHandler(m_fileSystemWatcher_Changed);
            m_sdkFileSystemWatcher.Created += new FileSystemEventHandler(m_fileSystemWatcher_Created);
            m_sdkFileSystemWatcher.Deleted += new FileSystemEventHandler(m_fileSystemWatcher_Deleted);
            m_sdkFileSystemWatcher.Renamed += new RenamedEventHandler(m_fileSystemWatcher_Renamed);

            m_workingProcessOverlay = new WorkingProcessOverlay();
            m_workingProcessOverlay.Dock = DockStyle.Fill;
            m_workingProcessOverlay.Visible = false;
            Controls.Add(m_workingProcessOverlay);

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

        public void SaveSceneBackup()
        {
            if (m_scenePage != null)
                m_scenePage.SaveSceneBackup();
        }

        public void RefreshSceneView()
        {
            if (m_scenePage != null)
                m_scenePage.RefreshSceneView();
        }

        public void ExploreGameObjectProperties(int handle, bool isPrefab)
        {
            if (m_rightPanel == null)
                return;

            m_rightPanel.Content.Controls.Clear();

            if (handle != 0)
            {
                GameObjectPropertiesControl editor = new GameObjectPropertiesControl(handle, isPrefab);
                editor.Dock = DockStyle.Fill;
                m_rightPanel.Content.Controls.Add(editor);
                m_rightPanel.Unroll();
            }
        }

        public void ExploreAssetsProperties(SceneViewPlugin.AssetType assetType)
        {
            if (m_rightPanel == null)
                return;

            m_rightPanel.Content.Controls.Clear();

            AssetsControl editor = new AssetsControl(assetType, ProjectModel);
            editor.Dock = DockStyle.Fill;
            m_rightPanel.Content.Controls.Add(editor);
            m_rightPanel.Unroll();
        }

        public void ExploreConfigProperties(string path)
        {
            if (m_rightPanel == null)
                return;

            m_rightPanel.Content.Controls.Clear();

            if (!File.Exists(path))
                return;

            ConfigControl editor = new ConfigControl(path, ProjectModel);
            editor.Dock = DockStyle.Fill;
            m_rightPanel.Content.Controls.Add(editor);
            m_rightPanel.Unroll();
        }

        public void UpdateGameObjectProperties()
        {
            if (m_rightPanel == null || m_rightPanel.Content.Controls.Count == 0 || !(m_rightPanel.Content.Controls[0] is GameObjectPropertiesControl))
                return;

            GameObjectPropertiesControl editor = m_rightPanel.Content.Controls[0] as GameObjectPropertiesControl;
            editor.UpdateEditorsValues();
        }

        public void WatchProjectFileSystem()
        {
            UnwatchProjectFileSystem();
            if (ProjectModel == null || !Directory.Exists(ProjectModel.WorkingDirectory))
                return;

            m_projectFileSystemWatcher.Path = ProjectModel.WorkingDirectory;
            m_projectFileSystemWatcher.EnableRaisingEvents = true;
        }

        public void UnwatchProjectFileSystem()
        {
            m_projectFileSystemWatcher.EnableRaisingEvents = false;
        }

        public void WatchSdkFileSystem()
        {
            UnwatchSdkFileSystem();
            if (SettingsModel == null || !Directory.Exists(SettingsModel.SdkPath))
                return;

            m_sdkFileSystemWatcher.Path = SettingsModel.SdkPath + @"\include\Ptakopysk";
            m_sdkFileSystemWatcher.EnableRaisingEvents = true;
        }

        public void UnwatchSdkFileSystem()
        {
            m_sdkFileSystemWatcher.EnableRaisingEvents = false;
        }

        public void InitializeGameEditorPages()
        {
            WatchProjectFileSystem();
            WatchSdkFileSystem();

            InitializeScenePage();
            InitializeBuildPage();
            //InitializeEditPage();
        }

        public void DeinitializeGameEditorPages()
        {
            UnwatchProjectFileSystem();
            UnwatchSdkFileSystem();

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
                this.DoOnUiThread(() => this.OnAction(action));
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
                    this.DoOnUiThread(() => this.OnAction(action));
                m_actionsQueue.Clear();
            }
        }

        public void LoadMetaFilesFrom(string dir)
        {
            DirectoryInfo info = new DirectoryInfo(dir);
            if (!info.Exists)
                return;

            DirectoryInfo[] dirs = info.GetDirectories();
            if (dirs != null && dirs.Length > 0)
                foreach (DirectoryInfo d in dirs)
                    LoadMetaFilesFrom(d.FullName);
            FileInfo[] files = info.GetFiles("*.meta");
            if (files != null && files.Length > 0)
            {
                foreach (FileInfo f in files)
                {
                    DoAction(new Action("LoadMetaComponent", f.FullName.Substring(0, f.FullName.Length - 5)), true);
                    DoAction(new Action("LoadMetaAsset", f.FullName.Substring(0, f.FullName.Length - 5)), true);
                }
            }
        }

        public void LoadSdkMetaFiles()
        {
            if (SettingsModel != null)
                LoadMetaFilesFrom(SettingsModel.SdkPath + @"\include\Ptakopysk");
        }

        public void GenerateProjectMetaFiles()
        {
            if (ProjectModel == null)
                return;

            ProjectModel.MetaComponentPaths.Clear();
            foreach (string file in ProjectModel.Files)
                GenerateMetaFile(file);
        }

        public void GenerateMetaFile(string path)
        {
            if (!File.Exists(path) || Path.GetExtension(path) != ".h")
                return;

            try
            {
                string content = File.ReadAllText(path);
                string log = "";
                string json = MetaCpp.GenerateMetaComponentJson(content, out log);
                if (String.IsNullOrEmpty(json))
                    DoAction(new Action("RemoveMetaComponent", path), true);
                else
                {
                    File.WriteAllText(path + ".meta", json);
                    DoAction(new Action("LoadMetaComponent", path), true);
                }
                json = MetaCpp.GenerateMetaAssetJson(content, out log);
                if (String.IsNullOrEmpty(json))
                    DoAction(new Action("RemoveMetaAsset", path), true);
                else
                {
                    File.WriteAllText(path + ".meta", json);
                    DoAction(new Action("LoadMetaAsset", path), true);
                }
            }
            catch { }
        }

        public void RemoveMetaFile(string path)
        {
            if (!File.Exists(path) || Path.GetExtension(path) != ".h" || !File.Exists(path + ".meta"))
                return;

            File.Delete(path + ".meta");
            DoAction(new Action("RemoveMetaComponent", path), true);
            DoAction(new Action("RemoveMetaAsset", path), true);
        }

        public void RenameMetaFile(string oldPath, string newPath)
        {
            DoAction(new Action("RemoveMetaComponent", oldPath), true);
            DoAction(new Action("RemoveMetaAsset", oldPath), true);

            if (!File.Exists(newPath) || Path.GetExtension(newPath) != ".h" || !File.Exists(oldPath + ".meta"))
                return;

            File.Move(oldPath + ".meta", newPath + ".meta");
            DoAction(new Action("LoadMetaComponent", newPath), true);
            DoAction(new Action("LoadMetaAsset", newPath), true);
        }

        public void OpenEditFile(string path, int line = -1)
        {
            if (SettingsModel == null || ProjectModel == null)
                return;

            string ext = Path.GetExtension(path);
            OpenFileWithDialog dialog = null;
            if (ext == ".h" || ext == ".cpp")
                dialog = new OpenFileWithDialog(APP_CODE_BLOCKS, "Default Code Editor");
            else if (ext == ".json")
                dialog = new OpenFileWithDialog(new string[] { APP_SCENE_EDITOR, APP_CONFIG_EDITOR }, "Default JSON Editor");
            else
                dialog = new OpenFileWithDialog();
            DialogResult result = dialog.OptionsCount > 0 ? dialog.ShowDialog() : DialogResult.OK;

            if (result == DialogResult.OK)
            {
                if (dialog.ResultOption == APP_CODE_BLOCKS)
                {
                    string cbExe = SettingsModel.CodeBlocksIdePath + @"\codeblocks.exe";
                    if (!File.Exists(cbExe))
                    {
                        MetroFramework.MetroMessageBox.Show(this, "Code::Blocks executable not found!", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                        return;
                    }

                    Process proc = new Process();
                    ProcessStartInfo info = new ProcessStartInfo();
                    info.WorkingDirectory = Path.GetFullPath(ProjectModel.WorkingDirectory);
                    info.FileName = Path.GetFullPath(cbExe);
                    info.Arguments = "--file=\"" + path + "\":" + (line < 0 ? "" : line.ToString()) + " " + ProjectModel.CbpPath;
                    proc.StartInfo = info;
                    proc.Start();
                }
                else if (dialog.ResultOption == APP_SCENE_EDITOR)
                {
                    if (m_scenePage != null)
                        if (m_scenePage.OpenScene(path))
                            SelectTabPage(TAB_NAME_SCENE);
                }
                else if (dialog.ResultOption == APP_CONFIG_EDITOR)
                    ExploreConfigProperties(path);
                else
                {
                    try
                    {
                        Process.Start(path);
                    }
                    catch (Exception ex)
                    {
                        MetroFramework.MetroMessageBox.Show(this, "Cannot open file: \"" + path + "\"!\n" + ex.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    }
                }
            }
        }

        public void RebuildEditorComponents(bool forced = false)
        {
            if (m_projectManagerPanel != null)
                m_projectManagerPanel.RebuildEditorPlugin(forced);
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
            InitializeRightPanel();
            InitializeBottomPanel();
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
            m_rightPanel.Docked += new EventHandler(sidePanel_DockUndock);
            m_rightPanel.Undocked += new EventHandler(sidePanel_DockUndock);
            m_mainPanel.Controls.Add(m_rightPanel);
            m_rightPanel.BringToFront();
            m_rightPanel.Content.Controls.Clear();
        }

        private void InitializeBottomPanel()
        {
            m_bottomPanel = new MetroSidePanel();
            MetroSkinManager.ApplyMetroStyle(m_bottomPanel);
            m_bottomPanel.Text = "Project Files";
            m_bottomPanel.Side = DockStyle.Bottom;
            m_bottomPanel.IsRolled = true;
            m_bottomPanel.AnimatedRolling = false;
            m_bottomPanel.Height = 200;
            m_bottomPanel.Width = m_mainPanel.Width;
            m_bottomPanel.Docked += new EventHandler(sidePanel_DockUndock);
            m_bottomPanel.Undocked += new EventHandler(sidePanel_DockUndock);
            m_mainPanel.Controls.Add(m_bottomPanel);
            m_bottomPanel.BringToFront();

            m_projectFilesPanel = new ProjectFilesControl();
            m_projectFilesPanel.Dock = DockStyle.Fill;
            m_bottomPanel.Content.Controls.Clear();
            m_bottomPanel.Content.Controls.Add(m_projectFilesPanel);
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
            m_settingsValidated = true;
            return true;
        }

        private void UpdateLayout()
        {
            int left = m_leftPanel != null && m_leftPanel.IsDocked ? m_leftPanel.Width - MetroSidePanel.ROLLED_PART_SIZE : 0;
            int right = m_rightPanel != null && m_rightPanel.IsDocked ? m_rightPanel.Width - MetroSidePanel.ROLLED_PART_SIZE : 0;
            int bottom = m_bottomPanel != null && m_bottomPanel.IsDocked ? m_bottomPanel.Height - MetroSidePanel.ROLLED_PART_SIZE : 0;
            m_mainPanelTabs.Left = m_mainPanel.Padding.Left + left;
            m_mainPanelTabs.Top = m_mainPanel.Padding.Top;
            m_mainPanelTabs.Width = m_mainPanel.Width - m_mainPanel.Padding.Horizontal - left - right;
            m_mainPanelTabs.Height = m_mainPanel.Height - m_mainPanel.Padding.Vertical - bottom;
            Padding p = m_leftPanel.OffsetPadding;
            p.Bottom = bottom + MetroSidePanel.ROLLED_PART_SIZE;
            m_leftPanel.OffsetPadding = p;
            m_leftPanel.Fit();
            m_leftPanel.Apply();
            p = m_rightPanel.OffsetPadding;
            p.Bottom = bottom + MetroSidePanel.ROLLED_PART_SIZE;
            m_rightPanel.OffsetPadding = p;
            m_rightPanel.Fit();
            m_rightPanel.Apply();
        }

        private void OnAction(Action action)
        {
            if (action == null)
                return;

            if (action.Id == "CbpChanged")
            {
                Console.WriteLine("CBP project file changed!");
                if (ProjectModel != null)
                {
                    ProjectModel.UpdateFromCbp();
                    MetaComponentsManager.Instance.UnregisterAllMetaComponents();
                    Parallel.Invoke(
                        () => LoadSdkMetaFiles(),
                        () => GenerateProjectMetaFiles()
                    );
                    if (m_buildPage != null)
                        m_buildPage.RefreshContent();
                    if (m_projectManagerPanel != null)
                        m_projectManagerPanel.RebuildList();
                }
            }
            else if (action.Id == "EditorCbpChanged")
            {
                Console.WriteLine("Editor CBP project file changed!");
                if (m_buildPage != null && !m_buildPage.IsBatchProcessRunning && ProjectModel != null && !string.IsNullOrEmpty(ProjectModel.EditorCbpPath))
                {
                    if (m_scenePage != null)
                        m_scenePage.SaveSceneBackup();
                    SceneViewPlugin.Unload();
                    m_buildPage.BatchOperationProject(
                        BuildPageControl.BatchOperationMode.Rebuild,
                        null,
                        ProjectModel.WorkingDirectory + @"\" + ProjectModel.EditorCbpPath
                        );
                }
            }
            else if (action.Id == "SceneViewPluginChanged")
            {
                Console.WriteLine("Editor components plugin changed!");
                if (ProjectModel != null && !string.IsNullOrEmpty(ProjectModel.EditorPluginPath))
                {
                    string pluginPath = ProjectModel.WorkingDirectory + @"\" + ProjectModel.EditorPluginPath;
                    SceneViewPlugin.Unload();
                    if (SceneViewPlugin.Load(pluginPath))
                    {
                        if (m_scenePage != null)
                            m_scenePage.ReinitializeRenderer();
                        List<string> clist = SceneViewPlugin.ListComponents();
                        if (clist != null && clist.Count > 0)
                            foreach (string c in clist)
                                Console.WriteLine("Registered component: " + c);
                        if (m_scenePage != null)
                            m_scenePage.OpenSceneBackup();
                    }
                }
            }
            else if (action.Id == "LoadMetaComponent" && action.Params != null && action.Params.Length > 0)
            {
                string path = action.Params[0] as string;
                if (!String.IsNullOrEmpty(path) && ProjectModel != null)
                {
                    Console.WriteLine("Load meta-component for: \"{0}\"", path);
                    string metaPath = path + ".meta";
                    if (File.Exists(metaPath))
                    {
                        string json = File.ReadAllText(metaPath);
                        MetaComponent meta = Newtonsoft.Json.JsonConvert.DeserializeObject<MetaComponent>(json);
                        MetaComponentsManager.Instance.UnregisterMetaComponent(meta);
                        ProjectModel.MetaComponentPaths.Remove(path);
                        if (meta != null)
                        {
                            MetaComponentsManager.Instance.RegisterMetaComponent(meta);
                            ProjectModel.MetaComponentPaths.Add(path, meta);
                            if (meta.Properties != null && meta.Properties.Count > 0)
                                foreach (MetaProperty prop in meta.Properties)
                                    if (PropertyEditorsManager.Instance.FindPropertyEditor(prop.ValueType) == null)
                                        Console.WriteLine("Property editor for type: \"{0}\" (component: \"{1}\", property: \"{2}\") not found!", prop.ValueType, meta.Name, prop.Name);
                        }
                    }
                    if (m_projectManagerPanel != null)
                        m_projectManagerPanel.UpdateFile(path);
                    GenerateProjectCodeFiles();
                }
            }
            else if (action.Id == "RemoveMetaComponent" && action.Params != null && action.Params.Length > 0)
            {
                string path = action.Params[0] as string;
                if (!String.IsNullOrEmpty(path) && ProjectModel != null && ProjectModel.MetaComponentPaths.ContainsKey(path))
                {
                    Console.WriteLine("Remove meta-component for: \"{0}\"", path);
                    MetaComponentsManager.Instance.UnregisterMetaComponent(ProjectModel.MetaComponentPaths[path]);
                    ProjectModel.MetaComponentPaths.Remove(path);
                    if (m_projectManagerPanel != null)
                        m_projectManagerPanel.UpdateFile(path);
                    GenerateProjectCodeFiles();
                }
            }
            else if (action.Id == "LoadMetaAsset" && action.Params != null && action.Params.Length > 0)
            {
                string path = action.Params[0] as string;
                if (!String.IsNullOrEmpty(path) && ProjectModel != null)
                {
                    Console.WriteLine("Load meta-asset for: \"{0}\"", path);
                    string metaPath = path + ".meta";
                    if (File.Exists(metaPath))
                    {
                        string json = File.ReadAllText(metaPath);
                        MetaAsset meta = Newtonsoft.Json.JsonConvert.DeserializeObject<MetaAsset>(json);
                        MetaAssetsManager.Instance.UnregisterMetaAsset(meta);
                        ProjectModel.MetaAssetsPaths.Remove(path);
                        if (meta != null)
                        {
                            MetaAssetsManager.Instance.RegisterMetaAsset(meta);
                            ProjectModel.MetaAssetsPaths.Add(path, meta);
                        }
                    }
                    if (m_projectManagerPanel != null)
                        m_projectManagerPanel.UpdateFile(path);
                    GenerateProjectCodeFiles();
                }
            }
            else if (action.Id == "RemoveMetaAsset" && action.Params != null && action.Params.Length > 0)
            {
                string path = action.Params[0] as string;
                if (!String.IsNullOrEmpty(path) && ProjectModel != null && ProjectModel.MetaAssetsPaths.ContainsKey(path))
                {
                    Console.WriteLine("Remove meta-asset for: \"{0}\"", path);
                    MetaAssetsManager.Instance.UnregisterMetaAsset(ProjectModel.MetaAssetsPaths[path]);
                    ProjectModel.MetaAssetsPaths.Remove(path);
                    if (m_projectManagerPanel != null)
                        m_projectManagerPanel.UpdateFile(path);
                    GenerateProjectCodeFiles();
                }
            }
            else if (action.Id == "GameObjectIdChanged" && action.Params != null && action.Params.Length > 0)
            {
                if (m_scenePage != null)
                    m_scenePage.SceneTreeChangeGameObjectId((int)action.Params[0]);
            }
        }

        private void GenerateProjectCodeFiles()
        {
            if (ProjectModel == null)
                return;

            string includeCoponents = "";
            string registerComponents = "";
            string includeAssets = "";
            string registerAssets = "";
            foreach (var kv in ProjectModel.MetaComponentPaths)
            {
                if (!kv.Key.StartsWith(ProjectModel.WorkingDirectory + @"\"))
                    continue;

                includeCoponents += "#include \"" + kv.Key + "\"\n";
                registerComponents += "Ptakopysk::GameManager::registerComponentFactory( \"" + kv.Value.Name + "\", RTTI_CLASS_TYPE( " + kv.Value.Name + " ), " + kv.Value.Name + "::onBuildComponent );\n";
            }
            foreach (var kv in ProjectModel.MetaAssetsPaths)
            {
                if (!kv.Key.StartsWith(ProjectModel.WorkingDirectory + @"\"))
                    continue;

                includeAssets += "#include \"" + kv.Key + "\"\n";
                registerAssets += "Ptakopysk::Assets::use().registerCustomAssetFactory( \"" + kv.Value.Name + "\", RTTI_CLASS_TYPE( " + kv.Value.Name + " ), " + kv.Value.Name + "::onBuildCustomAsset );\n";
            }

            string path = ProjectModel.WorkingDirectory + @"\" + ProjectModel.INCLUDE_COMPONENTS_FILE;
            if (File.Exists(path))
            {
                if (File.ReadAllText(path) != includeCoponents)
                    File.WriteAllText(path, includeCoponents);
            }
            else
                File.WriteAllText(path, includeCoponents);

            path = ProjectModel.WorkingDirectory + @"\" + ProjectModel.REGISTER_COMPONENTS_FILE;
            if (File.Exists(path))
            {
                if (File.ReadAllText(path) != registerComponents)
                    File.WriteAllText(path, registerComponents);
            }
            else
                File.WriteAllText(path, registerComponents);

            path = ProjectModel.WorkingDirectory + @"\" + ProjectModel.INCLUDE_ASSETS_FILE;
            if (File.Exists(path))
            {
                if (File.ReadAllText(path) != includeAssets)
                    File.WriteAllText(path, includeAssets);
            }
            else
                File.WriteAllText(path, includeAssets);

            path = ProjectModel.WorkingDirectory + @"\" + ProjectModel.REGISTER_ASSETS_FILE;
            if (File.Exists(path))
            {
                if (File.ReadAllText(path) != registerAssets)
                    File.WriteAllText(path, registerAssets);
            }
            else
                File.WriteAllText(path, registerAssets);
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
                if (m_rightPanel != null)
                {
                    m_rightPanel.IsRolled = SettingsModel.RightPanelRolled;
                    m_rightPanel.IsDocked = SettingsModel.RightPanelDocked;
                }
                if (m_bottomPanel != null)
                {
                    m_bottomPanel.IsRolled = SettingsModel.BottomPanelRolled;
                    m_bottomPanel.IsDocked = SettingsModel.BottomPanelDocked;
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
                if (m_rightPanel != null)
                {
                    SettingsModel.RightPanelRolled = m_rightPanel.IsRolled;
                    SettingsModel.RightPanelDocked = m_rightPanel.IsDocked;
                }
                if (m_bottomPanel != null)
                {
                    SettingsModel.BottomPanelRolled = m_bottomPanel.IsRolled;
                    SettingsModel.BottomPanelDocked = m_bottomPanel.IsDocked;
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
            Invalidate(true);
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
                else if (e.FullPath == ProjectModel.WorkingDirectory + @"\" + ProjectModel.EditorCbpPath)
                    DoAction(new Action("EditorCbpChanged"));
                else if (e.FullPath == ProjectModel.WorkingDirectory + @"\" + ProjectModel.EditorPluginPath)
                    DoAction(new Action("SceneViewPluginChanged"));
                else if (Path.GetExtension(e.FullPath) == ".h" && ProjectModel.Files.Contains(e.FullPath))
                    Parallel.Invoke(() => GenerateMetaFile(e.FullPath));
            }
        }

        private void m_fileSystemWatcher_Created(object sender, FileSystemEventArgs e)
        {
            if (ProjectModel != null)
            {
                if (e.FullPath == ProjectModel.WorkingDirectory + @"\" + ProjectModel.EditorCbpPath)
                    DoAction(new Action("EditorCbpChanged"));
                else if (e.FullPath == ProjectModel.WorkingDirectory + @"\" + ProjectModel.EditorPluginPath)
                    DoAction(new Action("SceneViewPluginChanged"));
                else if ((Path.GetExtension(e.FullPath) == ".h" || Path.GetExtension(e.FullPath) == ".cpp") && !e.FullPath.StartsWith(Path.GetDirectoryName(ProjectModel.WorkingDirectory + @"\" + ProjectModel.EditorCbpPath)))
                {
                    string cppFile = Path.ChangeExtension(e.FullPath, ".cpp");
                    string hFile = Path.ChangeExtension(e.FullPath, ".h");
                    if (File.Exists(cppFile) && !ProjectModel.Files.Contains(cppFile))
                        ProjectModel.Files.Add(cppFile);
                    if (File.Exists(hFile) && !ProjectModel.Files.Contains(hFile))
                        ProjectModel.Files.Add(hFile);
                    if(e.FullPath == hFile)
                        GenerateMetaFile(hFile);
                    ProjectModel.ApplyToCbp(SettingsModel);
                }
                if (m_projectFilesPanel != null)
                    m_projectFilesPanel.DoOnUiThread(() => m_projectFilesPanel.RebuildList());
            }
        }

        private void m_fileSystemWatcher_Deleted(object sender, FileSystemEventArgs e)
        {
            if (ProjectModel != null)
            {
                if ((Path.GetExtension(e.FullPath) == ".h" || Path.GetExtension(e.FullPath) == ".cpp") && ProjectModel.Files.Contains(e.FullPath))
                {
                    string hfile = Path.ChangeExtension(e.FullPath, ".h");
                    string cppfile = Path.ChangeExtension(e.FullPath, ".cpp");
                    RemoveMetaFile(hfile);
                    ProjectModel.Files.Remove(hfile);
                    ProjectModel.Files.Remove(cppfile);
                    ProjectModel.ApplyToCbp(SettingsModel);
                    if (File.Exists(hfile))
                        File.Delete(hfile);
                    if (File.Exists(cppfile))
                        File.Delete(cppfile);
                }
                if (m_projectFilesPanel != null)
                    m_projectFilesPanel.DoOnUiThread(() => m_projectFilesPanel.RebuildList());
            }
        }

        private void m_fileSystemWatcher_Renamed(object sender, RenamedEventArgs e)
        {
            if (ProjectModel != null)
            {
                if (Path.GetExtension(e.OldFullPath) == ".h" && ProjectModel.Files.Contains(e.OldFullPath))
                {
                    string cppPath = Path.ChangeExtension(e.OldFullPath, ".cpp");
                    if (File.Exists(cppPath))
                        File.Move(cppPath, Path.ChangeExtension(e.FullPath, ".cpp"));
                    ProjectModel.Files.Remove(e.OldFullPath);
                    if (Path.GetExtension(e.FullPath) == ".h")
                    {
                        ProjectModel.Files.Add(e.FullPath);
                        RenameMetaFile(e.OldFullPath, e.FullPath);
                    }
                    else
                        RemoveMetaFile(e.OldFullPath);
                    ProjectModel.ApplyToCbp(SettingsModel);
                }
                else if (Path.GetExtension(e.OldFullPath) == ".cpp" && ProjectModel.Files.Contains(e.OldFullPath))
                {
                    ProjectModel.Files.Remove(e.OldFullPath);
                    if (Path.GetExtension(e.FullPath) == ".cpp")
                    {
                        ProjectModel.Files.Add(e.FullPath);
                        string hPath = Path.ChangeExtension(e.OldFullPath, ".h");
                        if (File.Exists(hPath))
                            File.Move(hPath, Path.ChangeExtension(e.FullPath, ".h"));
                    }
                    ProjectModel.ApplyToCbp(SettingsModel);
                }
                if (m_projectFilesPanel != null)
                    m_projectFilesPanel.DoOnUiThread(() => m_projectFilesPanel.RebuildList());
            }
        }

        private void sidePanel_DockUndock(object sender, EventArgs e)
        {
            UpdateLayout();
        }

        #endregion
    }
}
