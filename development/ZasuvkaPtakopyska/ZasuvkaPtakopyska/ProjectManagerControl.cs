using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using MetroFramework.Controls;
using System.IO;
using System.Windows.Forms;
using MetroFramework;
using System.Diagnostics;

namespace ZasuvkaPtakopyska
{
    public class ProjectManagerControl : MetroPanel
    {
        #region Construction and Destruction.

        public ProjectManagerControl()
        {
            MetroSkinManager.ApplyMetroStyle(this);
            AutoScroll = true;
        }

        #endregion



        #region Public Functionality.

        public void RebuildList()
        {
            Controls.Clear();

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
                Controls.Add(btn);

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



        #region Private Events Handlers.

        private void btn_Click(object sender, EventArgs e)
        {
            MetroButton btn = sender as MetroButton;
            if (btn != null)
                OpenEditFile(btn.Tag as string);
        }
        
        #endregion
    }
}
