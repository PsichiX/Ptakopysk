using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using MetroFramework.Controls;
using System.Drawing;
using MetroFramework;
using System.IO;
using System.Diagnostics;
using System.Windows.Forms;
using ZasuvkaPtakopyskaExtender;

namespace ZasuvkaPtakopyska
{
    public class BuildPageControl : MetroPanel
    {
        #region Public Enumerators.

        public enum BatchOperationMode
        {
            Build,
            Rebuild,
            Clean
        }
        
        #endregion



        #region Private Static Data

        private static readonly Size DEFAULT_TILE_SIZE = new Size(128, 128);
        private static readonly Point DEFAULT_TILE_SEPARATOR = new Point(8, 8);
        
        #endregion



        #region Private Data.

        private Process m_runningProcess;
        private MetroComboBox m_activeBuildComboBox;
        private MetroProgressSpinner m_progressSpinner;
        private MetroTileIcon m_buildTile;
        private MetroTileIcon m_rebuildTile;
        private MetroTileIcon m_cleanTile;
        private MetroTileIcon m_buildAndRunTile;
        private MetroTileIcon m_runTile;
        private MetroTileIcon m_syncTile;
        private Queue<Action> m_afterBatchBuildQueue = new Queue<Action>();
        
        #endregion



        #region Construction and Destruction.

        public BuildPageControl()
        {
            MetroSkinManager.ApplyMetroStyle(this);
            AutoScroll = true;
            
            MetroLabel title = new MetroLabel();
            MetroSkinManager.ApplyMetroStyle(title);
            title.Text = "Active target: ";
            title.Size = new Size();
            title.AutoSize = true;
            title.Location = new Point(64, 64);
            Controls.Add(title);

            m_activeBuildComboBox = new MetroComboBox();
            MetroSkinManager.ApplyMetroStyle(m_activeBuildComboBox);
            m_activeBuildComboBox.BindingContext = new BindingContext();
            m_activeBuildComboBox.Location = new Point(title.Right, title.Top);
            m_activeBuildComboBox.SelectedValueChanged += new EventHandler(m_activeBuildComboBox_SelectedValueChanged);
            Controls.Add(m_activeBuildComboBox);

            m_progressSpinner = new MetroProgressSpinner();
            MetroSkinManager.ApplyMetroStyle(m_progressSpinner);
            m_progressSpinner.Visible = false;
            m_progressSpinner.Size = new Size(m_activeBuildComboBox.Height, m_activeBuildComboBox.Height);
            m_progressSpinner.Value = -1;
            m_progressSpinner.Location = new Point(m_activeBuildComboBox.Right + DEFAULT_TILE_SEPARATOR.X, m_activeBuildComboBox.Top);
            Controls.Add(m_progressSpinner);

            m_buildTile = new MetroTileIcon();
            MetroSkinManager.ApplyMetroStyle(m_buildTile);
            m_buildTile.Text = "BUILD";
            m_buildTile.Image = Bitmap.FromFile("resources/icons/appbar.cog.png");
            m_buildTile.Size = DEFAULT_TILE_SIZE;
            m_buildTile.Location = new Point(64, m_activeBuildComboBox.Bottom + DEFAULT_TILE_SEPARATOR.Y + DEFAULT_TILE_SEPARATOR.Y);
            m_buildTile.Click += new EventHandler(m_buildTile_Click);
            Controls.Add(m_buildTile);

            m_rebuildTile = new MetroTileIcon();
            MetroSkinManager.ApplyMetroStyle(m_rebuildTile);
            m_rebuildTile.Text = "REBUILD";
            m_rebuildTile.Image = Bitmap.FromFile("resources/icons/appbar.cogs.png");
            m_rebuildTile.Size = DEFAULT_TILE_SIZE;
            m_rebuildTile.Location = new Point(m_buildTile.Right + DEFAULT_TILE_SEPARATOR.X, m_buildTile.Top);
            m_rebuildTile.Click += new EventHandler(m_rebuildTile_Click);
            Controls.Add(m_rebuildTile);

            m_cleanTile = new MetroTileIcon();
            MetroSkinManager.ApplyMetroStyle(m_cleanTile);
            m_cleanTile.Text = "CLEAN";
            m_cleanTile.Image = Bitmap.FromFile("resources/icons/appbar.delete.png");
            m_cleanTile.Size = DEFAULT_TILE_SIZE;
            m_cleanTile.Location = new Point(m_rebuildTile.Right + DEFAULT_TILE_SEPARATOR.X, m_rebuildTile.Top);
            m_cleanTile.Click += new EventHandler(m_cleanTile_Click);
            Controls.Add(m_cleanTile);

            m_buildAndRunTile = new MetroTileIcon();
            MetroSkinManager.ApplyMetroStyle(m_buildAndRunTile);
            m_buildAndRunTile.Text = "BUILD && RUN";
            m_buildAndRunTile.Image = Bitmap.FromFile("resources/icons/appbar.control.play.png");
            m_buildAndRunTile.Size = DEFAULT_TILE_SIZE;
            m_buildAndRunTile.Location = new Point(m_buildTile.Left, m_buildTile.Bottom + DEFAULT_TILE_SEPARATOR.Y);
            m_buildAndRunTile.Click += new EventHandler(m_buildAndRunTile_Click);
            Controls.Add(m_buildAndRunTile);

            m_runTile = new MetroTileIcon();
            MetroSkinManager.ApplyMetroStyle(m_runTile);
            m_runTile.Text = "RUN";
            m_runTile.Image = Bitmap.FromFile("resources/icons/appbar.control.play.png");
            m_runTile.Size = DEFAULT_TILE_SIZE;
            m_runTile.Location = new Point(m_buildAndRunTile.Right + DEFAULT_TILE_SEPARATOR.X, m_buildAndRunTile.Top);
            m_runTile.Click += new EventHandler(m_runTile_Click);
            Controls.Add(m_runTile);

            m_syncTile = new MetroTileIcon();
            MetroSkinManager.ApplyMetroStyle(m_syncTile);
            m_syncTile.Text = "SYNC WITH\nCODE::BLOCKS";
            m_syncTile.Image = Bitmap.FromFile("resources/icons/appbar.refresh.png");
            m_syncTile.Size = DEFAULT_TILE_SIZE;
            m_syncTile.Location = new Point(m_runTile.Right + DEFAULT_TILE_SEPARATOR.X, m_runTile.Top);
            m_syncTile.Click += new EventHandler(m_syncTile_Click);
            Controls.Add(m_syncTile);
        }

        #endregion



        #region Public Functionality.

        public void RefreshContent()
        {
            MainForm mainForm = FindForm() as MainForm;
            if (mainForm != null && mainForm.ProjectModel != null && mainForm.ProjectModel.BuildTargets != null)
            {
                List<string> targets = new List<string>();
                foreach (string[] item in mainForm.ProjectModel.BuildTargets)
                    targets.Add(item[0]);
                m_activeBuildComboBox.DataSource = targets;
                if (targets.Count > 0 && targets.Contains(mainForm.ProjectModel.ActiveTarget))
                    m_activeBuildComboBox.SelectedItem = mainForm.ProjectModel.ActiveTarget;
            }
            else
                m_activeBuildComboBox.DataSource = null;
        }

        public void BatchOperationProject(BatchOperationMode mode, Action afterBuildAction = null)
        {
            MainForm mainForm = FindForm() as MainForm;
            if (mainForm == null || mainForm.SettingsModel == null || mainForm.ProjectModel == null)
                return;

            if (m_runningProcess != null && !m_runningProcess.HasExited)
            {
                MetroMessageBox.Show(mainForm, "Cannot run " + mode.ToString() + " operation because another operation is running!", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }
            
            string cbExe = mainForm.SettingsModel.CodeBlocksIdePath + @"\codeblocks.exe";
            if (!File.Exists(cbExe))
            {
                MetroMessageBox.Show(mainForm, "Code::Blocks executable not found!", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            string op = "";
            if (mode == BatchOperationMode.Build)
                op = "--build";
            else if (mode == BatchOperationMode.Rebuild)
                op = "--rebuild";
            else if (mode == BatchOperationMode.Clean)
                op = "--clean";

            string target = "--target=\"" + mainForm.ProjectModel.ActiveTarget + "\"";
            
            Process proc = new Process();
            ProcessStartInfo info = new ProcessStartInfo();
            info.WorkingDirectory = Path.GetFullPath(mainForm.ProjectModel.WorkingDirectory);
            info.FileName = Path.GetFullPath(cbExe);
            info.Arguments = "/ns /na /nd --no-batch-window-close " + target + " " + op + " " + mainForm.ProjectModel.CbpPath;
            info.UseShellExecute = false;
            info.RedirectStandardOutput = true;
            proc.StartInfo = info;
            proc.EnableRaisingEvents = true;
            proc.Exited += new EventHandler(proc_Exited);
            m_runningProcess = proc;
            m_progressSpinner.Visible = true;
            proc.Start();

            if (afterBuildAction != null)
            {
                lock (m_afterBatchBuildQueue)
                {
                    m_afterBatchBuildQueue.Enqueue(afterBuildAction);
                }
            }
        }

        public void SyncOperationProject()
        {
            MainForm mainForm = FindForm() as MainForm;
            if (mainForm == null || mainForm.SettingsModel == null || mainForm.ProjectModel == null)
                return;

            string cbExe = mainForm.SettingsModel.CodeBlocksIdePath + @"\codeblocks.exe";
            if (!File.Exists(cbExe))
            {
                MetroMessageBox.Show(mainForm, "Code::Blocks executable not found!", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            Process proc = new Process();
            ProcessStartInfo info = new ProcessStartInfo();
            info.WorkingDirectory = Path.GetFullPath(mainForm.ProjectModel.WorkingDirectory);
            info.FileName = Path.GetFullPath(cbExe);
            info.Arguments = mainForm.ProjectModel.CbpPath;
            proc.StartInfo = info;
            proc.Start();
        }

        public void RunProject()
        {
            MainForm mainForm = FindForm() as MainForm;
            if (mainForm == null || mainForm.ProjectModel == null || mainForm.ProjectModel.BuildTargets == null || mainForm.ProjectModel.BuildTargets.Count <= 0)
                return;

            if (m_runningProcess != null && !m_runningProcess.HasExited)
            {
                MetroMessageBox.Show(mainForm, "Cannot run project because another operation is running!", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            int index = mainForm.ProjectModel.BuildTargets.FindIndex(item => item[0] == mainForm.ProjectModel.ActiveTarget);
            if (index < 0 || index >= mainForm.ProjectModel.BuildTargets.Count)
            {
                MetroMessageBox.Show(mainForm, "Target: \"" + mainForm.ProjectModel.ActiveTarget + "\" not found!", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            string exe = mainForm.ProjectModel.WorkingDirectory + @"\" + mainForm.ProjectModel.BuildTargets[index][1] + ".exe";
            if (!File.Exists(exe))
            {
                MetroMessageBox.Show(mainForm, "Project executable for target: \"" + mainForm.ProjectModel.ActiveTarget + "\" not found!", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            Process proc = new Process();
            ProcessStartInfo info = new ProcessStartInfo();
            info.WorkingDirectory = Path.GetFullPath(mainForm.ProjectModel.WorkingDirectory + @"\" + mainForm.ProjectModel.BuildTargets[index][2]);
            info.FileName = Path.GetFullPath(exe);
            info.UseShellExecute = false;
            proc.StartInfo = info;
            proc.Start();
        }
        
        #endregion



        #region Private Events Handlers.

        private void m_activeBuildComboBox_SelectedValueChanged(object sender, EventArgs e)
        {
            MainForm mainForm = FindForm() as MainForm;
            if (mainForm != null
                && mainForm.ProjectModel != null
                && mainForm.ProjectModel.BuildTargets != null
                && mainForm.ProjectModel.BuildTargets.Exists(item => item[0] == m_activeBuildComboBox.SelectedValue as string)
                )
                mainForm.ProjectModel.ActiveTarget = mainForm.ProjectModel.BuildTargets.Find(item => item[0] == m_activeBuildComboBox.SelectedValue as string)[0];
        }

        private void m_buildTile_Click(object sender, EventArgs e)
        {
            BatchOperationProject(BatchOperationMode.Build);
        }

        private void m_rebuildTile_Click(object sender, EventArgs e)
        {
            BatchOperationProject(BatchOperationMode.Rebuild);
        }

        private void m_cleanTile_Click(object sender, EventArgs e)
        {
            BatchOperationProject(BatchOperationMode.Clean);
        }

        private void m_buildAndRunTile_Click(object sender, EventArgs e)
        {
            BatchOperationProject(BatchOperationMode.Build, () => RunProject());
        }

        private void m_runTile_Click(object sender, EventArgs e)
        {
            RunProject();
        }

        private void m_syncTile_Click(object sender, EventArgs e)
        {
            SyncOperationProject();
        }

        private void proc_Exited(object sender, EventArgs e)
        {
            m_progressSpinner.doOnUIThread(() => {
                m_progressSpinner.Visible = false;
            });
            if (m_runningProcess != null)
            {
                string log = m_runningProcess.StandardOutput.ReadToEnd();
                Console.Write(log);
                MainForm mainForm = FindForm() as MainForm;
                if (m_runningProcess.StartInfo.RedirectStandardOutput && mainForm != null)
                {
                    if (m_runningProcess.ExitCode == 0)
                        MetroMessageBox.Show(mainForm, log, "Operation Complete!", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    else
                        MetroMessageBox.Show(mainForm, log, "Operation Error: " + m_runningProcess.ExitCode.ToString(), MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
            
            lock (m_afterBatchBuildQueue)
            {
                Action action = m_afterBatchBuildQueue.Dequeue();
                if (m_runningProcess.ExitCode == 0 && action != null)
                    action();
            }

            m_runningProcess = null;
        }

        #endregion
    }
}
