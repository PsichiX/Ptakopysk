using System;
using System.Collections.Generic;
using MetroFramework.Controls;
using System.IO;
using System.Windows.Forms;
using MetroFramework;
using System.Diagnostics;
using System.Drawing;
using ZasuvkaPtakopyskaExtender;
using System.Xml;
using PtakopyskMetaGenerator;

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
            MetroTile icon;
            MetroButton btn;
            int type = 0;
            foreach (string file in mainForm.ProjectModel.Files)
            {
                if (mainForm.ProjectModel.MetaComponentPaths.ContainsKey(file) ||
                    (Path.GetExtension(file) == ".cpp" && mainForm.ProjectModel.MetaComponentPaths.ContainsKey(Path.ChangeExtension(file, ".h")))
                    )
                    type = 1;
                else
                    type = 0;

                btn = new MetroButton();
                MetroSkinManager.ApplyMetroStyle(btn);
                btn.Text = Path.GetFileName(file);
                btn.Tag = file;
                btn.Location = new Point(btn.Height, y);
                btn.Width = Width - btn.Height;
                btn.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
                btn.Click += new EventHandler(btn_Click);
                btn.MouseUp += new MouseEventHandler(btn_MouseUp);
                m_filesPanel.Controls.Add(btn);

                icon = new MetroTile();
                MetroSkinManager.ApplyMetroStyle(icon);
                icon.Text = type == 1 ? "C" : "";
                icon.TextAlign = ContentAlignment.MiddleCenter;
                icon.Tag = file;
                icon.Location = new Point(0, y);
                icon.Size = new Size(btn.Height, btn.Height);
                icon.TileTextFontSize = MetroTileTextSize.Small;
                icon.TileTextFontWeight = MetroTileTextWeight.Bold;
                icon.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
                m_filesPanel.Controls.Add(icon);

                y = btn.Bottom;
            }
        }

        public void UpdateFile(string file)
        {
            MainForm mainForm = FindForm() as MainForm;
            if (mainForm == null || mainForm.ProjectModel == null || mainForm.ProjectModel.Files == null)
                return;

            string cppfile = Path.ChangeExtension(file, ".cpp");
            int type = 0;
            if (mainForm.ProjectModel.MetaComponentPaths.ContainsKey(file) ||
                (Path.GetExtension(file) == ".cpp" && mainForm.ProjectModel.MetaComponentPaths.ContainsKey(cppfile))
                )
                type = 1;
            foreach (Control c in m_filesPanel.Controls)
                if (c is MetroTile && c.Tag is string && (c.Tag.Equals(file) || c.Tag.Equals(cppfile)))
                    c.Text = type == 1 ? "C" : "";
        }

        public void RebuildEditorComponents(bool forced = false)
        {
            MainForm mainForm = FindForm() as MainForm;
            if (mainForm == null || mainForm.SettingsModel == null || mainForm.ProjectModel == null)
                return;

            string dir = mainForm.ProjectModel.WorkingDirectory + @"\editor";
            string pluginFileName = mainForm.ProjectModel.Name.Replace(" ", "-") + "-Editor";
            string pluginFilePath = @"editor\" + pluginFileName + ".dll";
            string editorCbpPath = @"editor\" + Path.GetFileNameWithoutExtension(mainForm.ProjectModel.CbpPath) + ".Editor.cbp";
            string cbpPath = mainForm.ProjectModel.WorkingDirectory + @"\" + mainForm.ProjectModel.CbpPath;
            if (!Directory.Exists(dir))
                Directory.CreateDirectory(dir);

            bool somethingChanged = false;
            if (!File.Exists(dir + @"\dllmain.cpp"))
            {
                File.Copy(mainForm.SettingsModel.SdkPath + @"\templates\dllmain.cpp", dir + @"\dllmain.cpp");
                somethingChanged = true;
            }
            string includeFilePath = dir + @"\__components_headers_list__generated__.h";
            string registerFilePath = dir + @"\__register_components__generated__.inl";
            string unregisterFilePath = dir + @"\__unregister_components__generated__.inl";
            string includeContent = "";
            string registerContent = "";
            string unregisterContent = "";
            MetaComponent meta;
            foreach (string key in mainForm.ProjectModel.MetaComponentPaths.Keys)
            {
                includeContent += "#include \"" + key + "\"\r\n";
                meta = mainForm.ProjectModel.MetaComponentPaths[key];
                if (meta != null)
                {
                    registerContent += "REGISTER_COMPONENT( \"" + meta.Name + "\", " + meta.Name + " );\r\n";
                    unregisterContent += "UNREGISTER_COMPONENT( \"" + meta.Name + "\" );\r\n";
                }
            }
            // dummy way to check if generated files have the same content as new ones.
            if (!File.Exists(includeFilePath) || File.ReadAllText(includeFilePath) != includeContent)
            {
                File.WriteAllText(includeFilePath, includeContent);
                somethingChanged = true;
            }
            if (!File.Exists(registerFilePath) || File.ReadAllText(registerFilePath) != registerContent)
            {
                File.WriteAllText(registerFilePath, registerContent);
                somethingChanged = true;
            }
            if (!File.Exists(unregisterFilePath) || File.ReadAllText(unregisterFilePath) != unregisterContent)
            {
                File.WriteAllText(unregisterFilePath, unregisterContent);
                somethingChanged = true;
            }
            if (!forced && !somethingChanged && File.Exists(mainForm.ProjectModel.WorkingDirectory + @"\" + pluginFilePath))
            {
                mainForm.ProjectModel.EditorCbpPath = editorCbpPath;
                mainForm.ProjectModel.EditorComponentsPluginPath = pluginFilePath;
                mainForm.DoAction(new MainForm.Action("EditorComponentsPluginChanged"));
                return;
            }

            if (!File.Exists(cbpPath))
                return;

            string xml = File.ReadAllText(cbpPath);
            XmlDocument doc = new XmlDocument();
            doc.LoadXml(xml);
            XmlNode node;
            XmlAttribute attr;

            XmlNode project = doc.SelectSingleNode("CodeBlocks_project_file/Project");
            foreach (XmlNode pnode in project.ChildNodes)
                if (pnode.LocalName == "Option")
                    foreach (XmlAttribute pattr in pnode.Attributes)
                        if (pattr.LocalName == "title")
                            pattr.Value = mainForm.ProjectModel.Name + " - Editor";
            for (int i = project.ChildNodes.Count - 1; i >= 0; --i)
            {
                node = project.ChildNodes[i];
                if (node.LocalName == "Unit")
                    project.RemoveChild(node);
            }
            foreach (string key in mainForm.ProjectModel.Files)
            {
                node = doc.CreateElement("Unit");
                attr = doc.CreateAttribute("filename");
                attr.Value = key;
                node.Attributes.Append(attr);
                project.AppendChild(node);
            }
            node = doc.CreateElement("Unit");
            attr = doc.CreateAttribute("filename");
            attr.Value = "dllmain.cpp";
            node.Attributes.Append(attr);
            project.AppendChild(node);

            XmlNode build = doc.SelectSingleNode("CodeBlocks_project_file/Project/Build");
            foreach (XmlNode tnode in build.ChildNodes)
            {
                if (tnode.LocalName == "Target")
                {
                    foreach (XmlNode _tnode in tnode.ChildNodes)
                    {
                        bool isDebug = false;
                        if (_tnode.LocalName == "Option")
                        {
                            foreach (XmlAttribute _tattr in _tnode.Attributes)
                            {
                                if (_tattr.LocalName == "output")
                                    _tattr.Value = pluginFileName;
                                else if (_tattr.LocalName == "type")
                                    _tattr.Value = "3";
                            }
                        }
                        else if (_tnode.LocalName == "Compiler")
                        {
                            foreach (XmlNode __tnode in _tnode.ChildNodes)
                                if (__tnode.LocalName == "Add")
                                    foreach (XmlAttribute __tattr in __tnode.Attributes)
                                        if (__tattr.LocalName == "option" && __tattr.Value == "-g")
                                            isDebug = true;
                            node = doc.CreateElement("Add");
                            attr = doc.CreateAttribute("option");
                            attr.Value = "-Wall";
                            node.Attributes.Append(attr);
                            _tnode.AppendChild(node);
                            node = doc.CreateElement("Add");
                            attr = doc.CreateAttribute("option");
                            attr.Value = "-DPTAKOPYSK_EDITOR";
                            node.Attributes.Append(attr);
                            _tnode.AppendChild(node);
                        }
                        else if (_tnode.LocalName == "Linker")
                        {
                            node = doc.CreateElement("Add");
                            attr = doc.CreateAttribute("library");
                            attr.Value = isDebug ? "libPtakopyskInterface-d.a" : "libPtakopyskInterface.a";
                            node.Attributes.Append(attr);
                            _tnode.PrependChild(node);
                            node = doc.CreateElement("Add");
                            attr = doc.CreateAttribute("option");
                            attr.Value = "-Wl,-add-stdcall-alias";
                            node.Attributes.Append(attr);
                            _tnode.PrependChild(node);
                        }
                    }
                }
            }

            mainForm.ProjectModel.EditorCbpPath = editorCbpPath;
            mainForm.ProjectModel.EditorComponentsPluginPath = pluginFilePath;
            doc.Save(mainForm.ProjectModel.WorkingDirectory + @"\" + editorCbpPath);
        }

        #endregion



        #region Private Functionality.

        private void CreateNewComponent(string path, string name)
        {
            MainForm mainForm = FindForm() as MainForm;
            if (mainForm == null || mainForm.SettingsModel == null || mainForm.ProjectModel == null)
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
                {
                    DialogResult result = MetroMessageBox.Show(mainForm, "Open created component file?", "Open component", MessageBoxButtons.YesNo, MessageBoxIcon.Question);
                    if (result == DialogResult.Yes)
                        mainForm.OpenEditFile(hPath);
                }
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

            menuItem = new ToolStripMenuItem("Build Editor Components");
            menuItem.Click += new EventHandler(menuItem_buildEditorComponents_Click);
            menu.Items.Add(menuItem);

            menuItem = new ToolStripMenuItem("Rebuild Editor Components");
            menuItem.Click += new EventHandler(menuItem_rebuildEditorComponents_Click);
            menu.Items.Add(menuItem);

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
                prompt.Value = "UserComponent";
                result = prompt.ShowDialog();
                if (result == DialogResult.OK)
                    CreateNewComponent(dialog.SelectedPath, prompt.Value);
            }
        }

        private void menuItem_buildEditorComponents_Click(object sender, EventArgs e)
        {
            RebuildEditorComponents();
        }

        private void menuItem_rebuildEditorComponents_Click(object sender, EventArgs e)
        {
            RebuildEditorComponents(true);
        }

        private void btn_Click(object sender, EventArgs e)
        {
            MainForm mainForm = FindForm() as MainForm;
            MetroButton btn = sender as MetroButton;
            if (mainForm != null && btn != null && btn.Tag is string)
                mainForm.OpenEditFile(btn.Tag as string);
        }

        private void btn_MouseUp(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Right)
            {
                MetroButton btn = sender as MetroButton;

                MetroContextMenu menu = new MetroContextMenu(null);
                MetroSkinManager.ApplyMetroStyle(menu);
                ToolStripMenuItem menuItem;

                menuItem = new ToolStripMenuItem("Remove from project");
                menuItem.Tag = btn.Tag;
                menuItem.Click += new EventHandler(menuItem_removeFromProject_Click);
                menu.Items.Add(menuItem);

                menu.Show(btn, new Point(btn.Width, 0));
            }
        }

        private void menuItem_removeFromProject_Click(object sender, EventArgs e)
        {
            ToolStripMenuItem btn = sender as ToolStripMenuItem;
            MainForm mainForm = FindForm() as MainForm;
            if (btn != null && btn.Tag is string && mainForm != null && mainForm.ProjectModel != null && mainForm.ProjectModel.Files != null)
            {
                string hfile = Path.ChangeExtension(btn.Tag as string, ".h");
                if (mainForm.ProjectModel.Files.Contains(hfile) && File.Exists(hfile))
                    File.Delete(btn.Tag as string);
            }
        }

        #endregion
    }
}
