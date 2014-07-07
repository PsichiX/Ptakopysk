using System;
using System.Drawing;
using System.IO;
using System.Diagnostics;
using System.Windows.Forms;
using MetroFramework.Controls;
using Newtonsoft.Json;
using MetroFramework;

namespace ZasuvkaPtakopyska
{
    public partial class ProjectPageControl : MetroPanel
    {
        #region Private Static Data

        private static readonly Size DEFAULT_TILE_SIZE = new Size(128, 128);
        private static readonly Point DEFAULT_TILE_SEPARATOR = new Point(8, 8);
        private static readonly string DEFAULT_PROJECT_FILTER = "Zásuvka Ptakopyska project file (*.zasuvka)|*.zasuvka";
        
        #endregion



        #region Private Data.

        private ProjectModel m_projectModel;
        
        private MetroPanel m_generalPanel;
        private MetroTileIcon m_generalNewProjectTile;
        private MetroTileIcon m_generalOpenProjectTile;
        private MetroTileIcon m_generalImportProjectTile;

        private MetroPanel m_specificPanel;
        private MetroTileIcon m_specificSaveProjectTile;
        private MetroTileIcon m_specificExportProjectTile;
        private MetroTileIcon m_specificCloseProjectTile;
        
        #endregion



        #region Public Properties.

        public ProjectModel ProjectModel { get { return m_projectModel; } }

        #endregion



        #region Construction and Destruction.

        public ProjectPageControl()
        {
            MetroSkinManager.ApplyMetroStyle(this);
            AutoScroll = true;
            
            InitializeGeneral();
            InitializeSpecific();
        }

        #endregion



        #region Private Functionality.

        private void InitializeGeneral()
        {
            m_generalPanel = new MetroPanel();
            MetroSkinManager.ApplyMetroStyle(m_generalPanel);
            m_generalPanel.Size = new Size();
            m_generalPanel.AutoSize = true;
            m_generalPanel.Left = 64;
            m_generalPanel.Top = 64;
            Controls.Add(m_generalPanel);

            MetroLabel title = new MetroLabel();
            MetroSkinManager.ApplyMetroStyle(title);
            title.Text = "General";
            title.Size = new Size();
            title.AutoSize = true;
            m_generalPanel.Controls.Add(title);

            m_generalNewProjectTile = new MetroTileIcon();
            MetroSkinManager.ApplyMetroStyle(m_generalNewProjectTile);
            m_generalNewProjectTile.Text = "NEW\nPROJECT";
            m_generalNewProjectTile.Image = Bitmap.FromFile("resources/icons/appbar.page.new.png");
            m_generalNewProjectTile.Size = DEFAULT_TILE_SIZE;
            m_generalNewProjectTile.Location = new Point(DEFAULT_TILE_SEPARATOR.X, title.Bottom + DEFAULT_TILE_SEPARATOR.Y);
            m_generalNewProjectTile.Click += new EventHandler(m_generalNewProjectTile_Click);
            m_generalPanel.Controls.Add(m_generalNewProjectTile);

            m_generalOpenProjectTile = new MetroTileIcon();
            MetroSkinManager.ApplyMetroStyle(m_generalOpenProjectTile);
            m_generalOpenProjectTile.Text = "OPEN\nPROJECT";
            m_generalOpenProjectTile.Image = Bitmap.FromFile("resources/icons/appbar.page.edit.png");
            m_generalOpenProjectTile.Size = DEFAULT_TILE_SIZE;
            m_generalOpenProjectTile.Location = new Point(m_generalNewProjectTile.Right + DEFAULT_TILE_SEPARATOR.X, title.Bottom + DEFAULT_TILE_SEPARATOR.Y);
            m_generalOpenProjectTile.Click += new EventHandler(m_generalOpenProjectTile_Click);
            m_generalPanel.Controls.Add(m_generalOpenProjectTile);

            m_generalImportProjectTile = new MetroTileIcon();
            MetroSkinManager.ApplyMetroStyle(m_generalImportProjectTile);
            m_generalImportProjectTile.Text = "IMPORT\nPROJECT";
            m_generalImportProjectTile.Image = Bitmap.FromFile("resources/icons/appbar.page.download.png");
            m_generalImportProjectTile.Size = DEFAULT_TILE_SIZE;
            m_generalImportProjectTile.Location = new Point(m_generalOpenProjectTile.Right + DEFAULT_TILE_SEPARATOR.X, title.Bottom + DEFAULT_TILE_SEPARATOR.Y);
            m_generalImportProjectTile.Click += new EventHandler(m_generalImportProjectTile_Click);
            m_generalPanel.Controls.Add(m_generalImportProjectTile);
        }

        private void InitializeSpecific()
        {
            m_specificPanel = new MetroPanel();
            MetroSkinManager.ApplyMetroStyle(m_specificPanel);
            m_specificPanel.Size = new Size();
            m_specificPanel.AutoSize = true;
            m_specificPanel.Left = 64;
            m_specificPanel.Top = m_generalPanel.Bottom + 64;
            Controls.Add(m_specificPanel);

            MetroLabel title = new MetroLabel();
            MetroSkinManager.ApplyMetroStyle(title);
            title.Text = "Project-specific";
            title.Size = new Size();
            title.AutoSize = true;
            m_specificPanel.Controls.Add(title);

            m_specificSaveProjectTile = new MetroTileIcon();
            MetroSkinManager.ApplyMetroStyle(m_specificSaveProjectTile);
            m_specificSaveProjectTile.Text = "SAVE\nPROJECT";
            m_specificSaveProjectTile.Image = Bitmap.FromFile("resources/icons/appbar.save.png");
            m_specificSaveProjectTile.Size = DEFAULT_TILE_SIZE;
            m_specificSaveProjectTile.Location = new Point(DEFAULT_TILE_SEPARATOR.X, title.Bottom + DEFAULT_TILE_SEPARATOR.Y);
            m_specificSaveProjectTile.Click += new EventHandler(m_specificSaveProjectTile_Click);
            m_specificPanel.Controls.Add(m_specificSaveProjectTile);

            m_specificExportProjectTile = new MetroTileIcon();
            MetroSkinManager.ApplyMetroStyle(m_specificExportProjectTile);
            m_specificExportProjectTile.Text = "EXPORT\nPROJECT";
            m_specificExportProjectTile.Image = Bitmap.FromFile("resources/icons/appbar.page.upload.png");
            m_specificExportProjectTile.Size = DEFAULT_TILE_SIZE;
            m_specificExportProjectTile.Location = new Point(m_specificSaveProjectTile.Right + DEFAULT_TILE_SEPARATOR.X, title.Bottom + DEFAULT_TILE_SEPARATOR.Y);
            m_specificExportProjectTile.Click += new EventHandler(m_specificExportProjectTile_Click);
            m_specificPanel.Controls.Add(m_specificExportProjectTile);

            m_specificCloseProjectTile = new MetroTileIcon();
            MetroSkinManager.ApplyMetroStyle(m_specificCloseProjectTile);
            m_specificCloseProjectTile.Text = "CLOSE\nPROJECT";
            m_specificCloseProjectTile.Image = Bitmap.FromFile("resources/icons/appbar.close.png");
            m_specificCloseProjectTile.Size = DEFAULT_TILE_SIZE;
            m_specificCloseProjectTile.Location = new Point(m_specificExportProjectTile.Right + DEFAULT_TILE_SEPARATOR.X, title.Bottom + DEFAULT_TILE_SEPARATOR.Y);
            m_specificCloseProjectTile.Click += new EventHandler(m_specificCloseProjectTile_Click);
            m_specificPanel.Controls.Add(m_specificCloseProjectTile);
        }

        #endregion



        #region Public Functionality.

        public void CreateNewProject(string path, string name)
        {
            MainForm mainForm = FindForm() as MainForm;
            if (mainForm == null || mainForm.SettingsModel == null)
                return;

            if (!File.Exists(mainForm.SettingsModel.BashBinPath))
            {
                MetroMessageBox.Show(mainForm, "Bash executable not found!", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            path = Path.GetFullPath(path);
            string unixPath = Utils.ConvertWindowsToUnixPath(path);
            string wrkdir = mainForm.SettingsModel.SdkPath + @"\templates";
            Process proc = new Process();
            ProcessStartInfo info = new ProcessStartInfo();
            info.WorkingDirectory = Path.GetFullPath(wrkdir);
            info.FileName = Path.GetFullPath(mainForm.SettingsModel.BashBinPath);
            info.Arguments = "-l -c './make_new_project.sh -o \"" + unixPath + "\" -p \"" + name + "\"'";
            info.UseShellExecute = false;
            info.CreateNoWindow = true;
            //info.RedirectStandardOutput = true;
            proc.StartInfo = info;
            if (File.Exists(info.FileName))
            {
                proc.Start();
                proc.WaitForExit();
                if (Directory.Exists(path))
                {
                    ProjectModel projectModel = new ProjectModel(name + ".cbp");
                    projectModel.WorkingDirectory = Path.GetDirectoryName(path);
                    projectModel.UpdateFromCbp();
                    string json = JsonConvert.SerializeObject(projectModel, Formatting.Indented);
                    File.WriteAllText(path + @"\project.zasuvka", json);
                    OpenProject(path + @"\project.zasuvka");
                }
            }
        }

        public void OpenProject(string filePath)
        {
            filePath = Path.GetFullPath(filePath);
            if (!File.Exists(filePath))
                return;

            CloseProject();

            string json = File.ReadAllText(filePath);
            m_projectModel = JsonConvert.DeserializeObject<ProjectModel>(json);
            m_projectModel.WorkingDirectory = Path.GetDirectoryName(filePath);
            m_projectModel.UpdateFromCbp();
            
            MainForm mainForm = FindForm() as MainForm;
            if (mainForm != null)
            {
                m_projectModel.ApplyToCbp(mainForm.SettingsModel);

                mainForm.InitializeGameEditorPages();
                mainForm.SelectTabPage(MainForm.TAB_NAME_BUILD);
                mainForm.AppTitleExtended = m_projectModel.Name;
            }
        }

        public void SaveProject()
        {
            MainForm mainForm = FindForm() as MainForm;
            if (mainForm != null && m_projectModel != null)
            {
                if(m_projectModel.ApplyToCbp(mainForm.SettingsModel))
                    MetroMessageBox.Show(FindForm(), "Project \"" + m_projectModel.Name + "\" saved!", "Information", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }

        public void CloseProject()
        {
            m_projectModel = null;
            
            MainForm mainForm = FindForm() as MainForm;
            if (mainForm != null)
            {
                mainForm.DeinitializeGameEditorPages();
                mainForm.SelectTabPage(MainForm.TAB_NAME_PROJECT);
                mainForm.AppTitleExtended = null;
            }
        }
        
        #endregion



        #region Private Events Handlers.

        private void m_generalNewProjectTile_Click(object sender, EventArgs e)
        {
            FolderBrowserDialog dialog = new FolderBrowserDialog();
            dialog.ShowNewFolderButton = true;
            DialogResult result = dialog.ShowDialog();
            if (result == DialogResult.OK)
            {
                MetroPromptBox prompt = new MetroPromptBox();
                prompt.Title = "Enter project name:";
                prompt.Value = "Project";
                result = prompt.ShowDialog();
                if (result == DialogResult.OK)
                    CreateNewProject(dialog.SelectedPath, prompt.Value);
            }
        }

        private void m_generalOpenProjectTile_Click(object sender, EventArgs e)
        {
            OpenFileDialog dialog = new OpenFileDialog();
            dialog.CheckPathExists = true;
            dialog.RestoreDirectory = true;
            dialog.Filter = DEFAULT_PROJECT_FILTER;
            DialogResult result = dialog.ShowDialog();
            if (result == DialogResult.OK)
                OpenProject(dialog.FileName);
        }

        private void m_generalImportProjectTile_Click(object sender, EventArgs e)
        {
        }

        private void m_specificSaveProjectTile_Click(object sender, EventArgs e)
        {
            if (m_projectModel != null)
                SaveProject();
        }

        private void m_specificExportProjectTile_Click(object sender, EventArgs e)
        {
        }

        private void m_specificCloseProjectTile_Click(object sender, EventArgs e)
        {
            CloseProject();
        }

        #endregion
    }
}
