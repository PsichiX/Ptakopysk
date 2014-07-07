﻿using System;
using System.Collections.Generic;
using MetroFramework.Controls;
using System.IO;
using System.Windows.Forms;
using MetroFramework;
using System.Diagnostics;
using System.Drawing;

namespace ZasuvkaPtakopyska
{
    public class ProjectManagerControl : MetroPanel
    {
        #region Private Static Data.

        private static readonly int DEFAULT_SEPARATOR = 12;
        
        #endregion



        #region Private Data.

        private MetroTileIcon m_optionsTile;
        private MetroPanel m_filesPanel;
        
        #endregion



        #region Construction and Destruction.

        public ProjectManagerControl()
        {
            MetroSkinManager.ApplyMetroStyle(this);
            AutoScroll = true;

            m_optionsTile = new MetroTileIcon();
            MetroSkinManager.ApplyMetroStyle(m_optionsTile);
            m_optionsTile.Text = "Options";
            m_optionsTile.TextAlign = ContentAlignment.MiddleCenter;
            m_optionsTile.Top = DEFAULT_SEPARATOR;
            m_optionsTile.Width = Width;
            m_optionsTile.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
            m_optionsTile.Click += new EventHandler(m_optionsTile_Click);
            Controls.Add(m_optionsTile);

            m_filesPanel = new MetroPanel();
            MetroSkinManager.ApplyMetroStyle(m_filesPanel);
            m_filesPanel.Size = new Size(Width, 0);
            m_filesPanel.AutoSize = true;
            //m_filesPanel.AutoScroll = true;
            m_filesPanel.Top = m_optionsTile.Bottom + DEFAULT_SEPARATOR;
            m_filesPanel.Width = Width;
            m_filesPanel.Height = Height - m_filesPanel.Top;
            m_filesPanel.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
            Controls.Add(m_filesPanel);
        }

        #endregion



        #region Public Functionality.

        public void RebuildList()
        {
            m_filesPanel.Controls.Clear();

            MainForm mainForm = FindForm() as MainForm;
            if (mainForm == null || mainForm.ProjectModel == null || mainForm.ProjectModel.Files == null)
                return;

            int y = 0;
            MetroButton btn;
            foreach (string file in mainForm.ProjectModel.Files)
            {
                btn = new MetroButton();
                MetroSkinManager.ApplyMetroStyle(btn);
                btn.Text = Path.GetFileName(file);
                btn.Tag = file;
                btn.Top = y;
                btn.Width = Width;
                btn.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
                btn.Click += new EventHandler(btn_Click);
                m_filesPanel.Controls.Add(btn);

                y = btn.Bottom;
            }
        }

        public void OpenEditFile(string path, int line = -1)
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
            info.Arguments = "--file=\"" + path + "\":" + (line < 0 ? "" : line.ToString()) + " " + mainForm.ProjectModel.CbpPath;
            proc.StartInfo = info;
            proc.Start();
        }

        #endregion



        #region Private Functionality.

        private void CreateNewComponent(string path, string name)
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
            info.Arguments = "-l -c './make_new_component.sh -o \"" + unixPath + "\" -c \"" + name + "\"'";
            info.UseShellExecute = false;
            info.CreateNoWindow = true;
            //info.RedirectStandardOutput = true;
            proc.StartInfo = info;
            if (File.Exists(info.FileName))
            {
                proc.Start();
                proc.WaitForExit();
                string cppPath = path + @"\" + name + ".cpp";
                string hPath = path + @"\" + name + ".h";
                mainForm.ProjectModel.Files.Add(cppPath);
                mainForm.ProjectModel.Files.Add(hPath);
                mainForm.ProjectModel.ApplyToCbp(mainForm.SettingsModel);
                if (File.Exists(hPath))
                    OpenEditFile(hPath);
            }
        }
        
        #endregion



        #region Private Events Handlers.

        private void m_optionsTile_Click(object sender, EventArgs e)
        {
            MainForm mainForm = FindForm() as MainForm;
            if (mainForm == null || mainForm.ProjectModel == null)
                return;
            
            MetroContextMenu menu = new MetroContextMenu(null);
            MetroSkinManager.ApplyMetroStyle(menu);
            ToolStripMenuItem menuItem;
            
            menuItem = new ToolStripMenuItem("New C++ component");
            menuItem.Click += new EventHandler(menuItem_newCppComponent_Click);
            menu.Items.Add(menuItem);
            
            menu.Show(m_optionsTile, new Point(m_optionsTile.Width, 0));
        }

        private void menuItem_newCppComponent_Click(object sender, EventArgs e)
        {
            MainForm mainForm = FindForm() as MainForm;
            if (mainForm == null || mainForm.ProjectModel == null)
                return;

            FolderBrowserDialog dialog = new FolderBrowserDialog();
            dialog.ShowNewFolderButton = true;
            dialog.SelectedPath = mainForm.ProjectModel.WorkingDirectory;
            DialogResult result = dialog.ShowDialog();
            if (result == DialogResult.OK)
            {
                MetroPromptBox prompt = new MetroPromptBox();
                prompt.Title = "Enter component name:";
                prompt.Value = "Component";
                result = prompt.ShowDialog();
                if (result == DialogResult.OK)
                    CreateNewComponent(dialog.SelectedPath, prompt.Value);
            }
        }
        
        private void btn_Click(object sender, EventArgs e)
        {
            MetroButton btn = sender as MetroButton;
            if (btn != null)
                OpenEditFile(btn.Tag as string);
        }

        #endregion
    }
}
