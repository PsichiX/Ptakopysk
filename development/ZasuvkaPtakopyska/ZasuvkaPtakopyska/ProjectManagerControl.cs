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
                if (mainForm.ProjectModel.MetaAssetsPaths.ContainsKey(file) ||
                    (Path.GetExtension(file) == ".cpp" && mainForm.ProjectModel.MetaAssetsPaths.ContainsKey(Path.ChangeExtension(file, ".h")))
                    )
                    type = 2;
                else if (mainForm.ProjectModel.MetaComponentPaths.ContainsKey(file) ||
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
                if (type == 2)
                    icon.Text = "A";
                else if (type == 1)
                    icon.Text = "C";
                else
                    icon.Text = "";
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
            int type = -1;
            if (mainForm.ProjectModel.MetaAssetsPaths.ContainsKey(file) ||
                (Path.GetExtension(file) == ".cpp" && mainForm.ProjectModel.MetaAssetsPaths.ContainsKey(cppfile))
                )
                type = 2;
            else if (mainForm.ProjectModel.MetaComponentPaths.ContainsKey(file) ||
                (Path.GetExtension(file) == ".cpp" && mainForm.ProjectModel.MetaComponentPaths.ContainsKey(cppfile))
                )
                type = 1;
            else
                type = 0;

            foreach (Control c in m_filesPanel.Controls)
            {
                if (c is MetroTile && c.Tag is string && (c.Tag.Equals(file) || c.Tag.Equals(cppfile)))
                {
                    if (type == 2)
                        c.Text = "A";
                    else if (type == 1)
                        c.Text = "C";
                    else
                        c.Text = "";
                }
            }
        }

        public void RebuildEditorPlugin(bool forced = false)
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
            if (forced || !File.Exists(dir + @"\dllmain.cpp"))
            {
                if (!File.Exists(mainForm.SettingsModel.SdkPath + @"\templates\dllmain.cpp"))
                    return;

                File.Copy(mainForm.SettingsModel.SdkPath + @"\templates\dllmain.cpp", dir + @"\dllmain.cpp", true);
                somethingChanged = true;
            }
            string includeComponentsFilePath = dir + @"\__components_headers_list__generated__.h";
            string registerComponentsFilePath = dir + @"\__register_components__generated__.inl";
            string unregisterComponentsFilePath = dir + @"\__unregister_components__generated__.inl";
            string includeAssetsFilePath = dir + @"\__assets_headers_list__generated__.h";
            string registerAssetsFilePath = dir + @"\__register_assets__generated__.inl";
            string unregisterAssetsFilePath = dir + @"\__unregister_assets__generated__.inl";
            string includeComponentsContent = "";
            string registerComponentsContent = "";
            string unregisterComponentsContent = "";
            string includeAssetsContent = "";
            string registerAssetsContent = "";
            string unregisterAssetsContent = "";
            MetaComponent metaComponent;
            foreach (string key in mainForm.ProjectModel.MetaComponentPaths.Keys)
            {
                if (!key.StartsWith(mainForm.ProjectModel.WorkingDirectory + @"\"))
                    continue;

                includeComponentsContent += "#include \"" + key + "\"\r\n";
                metaComponent = mainForm.ProjectModel.MetaComponentPaths[key];
                if (metaComponent != null)
                {
                    registerComponentsContent += "REGISTER_COMPONENT( \"" + metaComponent.Name + "\", " + metaComponent.Name + " );\r\n";
                    unregisterComponentsContent += "UNREGISTER_COMPONENT( \"" + metaComponent.Name + "\" );\r\n";
                }
            }
            MetaAsset metaAsset;
            foreach (string key in mainForm.ProjectModel.MetaAssetsPaths.Keys)
            {
                if (!key.StartsWith(mainForm.ProjectModel.WorkingDirectory + @"\"))
                    continue;

                includeAssetsContent += "#include \"" + key + "\"\r\n";
                metaAsset = mainForm.ProjectModel.MetaAssetsPaths[key];
                if (metaAsset != null)
                {
                    registerAssetsContent += "REGISTER_ASSET( \"" + metaAsset.Name + "\", " + metaAsset.Name + " );\r\n";
                    unregisterAssetsContent += "UNREGISTER_ASSET( \"" + metaAsset.Name + "\" );\r\n";
                }
            }
            // dummy way to check if generated files have the same content as new ones.
            if (!File.Exists(includeComponentsFilePath) || File.ReadAllText(includeComponentsFilePath) != includeComponentsContent)
            {
                File.WriteAllText(includeComponentsFilePath, includeComponentsContent);
                somethingChanged = true;
            }
            if (!File.Exists(registerComponentsFilePath) || File.ReadAllText(registerComponentsFilePath) != registerComponentsContent)
            {
                File.WriteAllText(registerComponentsFilePath, registerComponentsContent);
                somethingChanged = true;
            }
            if (!File.Exists(unregisterComponentsFilePath) || File.ReadAllText(unregisterComponentsFilePath) != unregisterComponentsContent)
            {
                File.WriteAllText(unregisterComponentsFilePath, unregisterComponentsContent);
                somethingChanged = true;
            }
            if (!File.Exists(includeAssetsFilePath) || File.ReadAllText(includeAssetsFilePath) != includeAssetsContent)
            {
                File.WriteAllText(includeAssetsFilePath, includeAssetsContent);
                somethingChanged = true;
            }
            if (!File.Exists(registerAssetsFilePath) || File.ReadAllText(registerAssetsFilePath) != registerAssetsContent)
            {
                File.WriteAllText(registerAssetsFilePath, registerAssetsContent);
                somethingChanged = true;
            }
            if (!File.Exists(unregisterAssetsFilePath) || File.ReadAllText(unregisterAssetsFilePath) != unregisterAssetsContent)
            {
                File.WriteAllText(unregisterAssetsFilePath, unregisterAssetsContent);
                somethingChanged = true;
            }
            if (!somethingChanged && File.Exists(mainForm.ProjectModel.WorkingDirectory + @"\" + pluginFilePath))
            {
                mainForm.ProjectModel.EditorCbpPath = editorCbpPath;
                mainForm.ProjectModel.EditorPluginPath = pluginFilePath;
                mainForm.SaveSceneBackup();
                mainForm.DoAction(new MainForm.Action("SceneViewPluginChanged"));
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
                            attr.Value = isDebug ? "libSceneViewInterface-d.a" : "libSceneViewInterface.a";
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
            mainForm.ProjectModel.EditorPluginPath = pluginFilePath;
            doc.Save(mainForm.ProjectModel.WorkingDirectory + @"\" + editorCbpPath);
        }

        #endregion



        #region Private Functionality.

        private void CreateNewComponent(string path, string name)
        {
            MainForm mainForm = FindForm() as MainForm;
            if (mainForm == null || mainForm.SettingsModel == null || mainForm.ProjectModel == null)
                return;

            path = Path.GetFullPath(path);
            string wrkdir = mainForm.SettingsModel.SdkPath + @"\templates";

            Newtonsoft.Json.Linq.JObject args = new Newtonsoft.Json.Linq.JObject();
            args["inputPath"] = wrkdir;
            args["outputPath"] = path;
            args["name"] = name;
            args["nameUpper"] = name.ToUpperInvariant();
            if (TemplateFilesManager.ProcessTemplates("resources/settings/MakeNewComponent.json", args))
            {
                string cppPath = path + @"\" + name + ".cpp";
                string hPath = path + @"\" + name + ".h";
                mainForm.ProjectModel.Files.Add(cppPath);
                mainForm.ProjectModel.Files.Add(hPath);
                mainForm.ProjectModel.ApplyToCbp(mainForm.SettingsModel);
                if (File.Exists(hPath))
                {
                    DialogResult result = MetroMessageBox.Show(mainForm, "Open created component file?", "Open component to edit", MessageBoxButtons.YesNo, MessageBoxIcon.Question);
                    if (result == DialogResult.Yes)
                        mainForm.OpenEditFile(hPath);
                }
            }
        }

        private void CreateNewAsset(string path, string name)
        {
            MainForm mainForm = FindForm() as MainForm;
            if (mainForm == null || mainForm.SettingsModel == null || mainForm.ProjectModel == null)
                return;

            path = Path.GetFullPath(path);
            string wrkdir = mainForm.SettingsModel.SdkPath + @"\templates";
            Newtonsoft.Json.Linq.JObject args = new Newtonsoft.Json.Linq.JObject();
            args["inputPath"] = wrkdir;
            args["outputPath"] = path;
            args["name"] = name;
            args["nameUpper"] = name.ToUpperInvariant();
            if (TemplateFilesManager.ProcessTemplates("resources/settings/MakeNewAsset.json", args))
            {
                string cppPath = path + @"\" + name + ".cpp";
                string hPath = path + @"\" + name + ".h";
                mainForm.ProjectModel.Files.Add(cppPath);
                mainForm.ProjectModel.Files.Add(hPath);
                mainForm.ProjectModel.ApplyToCbp(mainForm.SettingsModel);
                if (File.Exists(hPath))
                {
                    DialogResult result = MetroMessageBox.Show(mainForm, "Open created asset code file?", "Open asset to edit", MessageBoxButtons.YesNo, MessageBoxIcon.Question);
                    if (result == DialogResult.Yes)
                        mainForm.OpenEditFile(hPath);
                }
            }
        }

        private void UpdateMainCpp()
        {
            MainForm mainForm = FindForm() as MainForm;
            if (mainForm == null || mainForm.SettingsModel == null || mainForm.ProjectModel == null)
                return;

            string path = mainForm.ProjectModel.WorkingDirectory;
            string name = mainForm.ProjectModel.Name;
            string wrkdir = mainForm.SettingsModel.SdkPath + @"\templates";
            Newtonsoft.Json.Linq.JObject args = new Newtonsoft.Json.Linq.JObject();
            args["inputPath"] = wrkdir;
            args["outputPath"] = path;
            args["name"] = name;
            TemplateFilesManager.ProcessTemplates("resources/settings/UpdateMainCpp.json", args);
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

            menuItem = new ToolStripMenuItem("Build Scene View plugin");
            menuItem.Click += new EventHandler(menuItem_buildSceneViewPlugin_Click);
            menu.Items.Add(menuItem);

            menuItem = new ToolStripMenuItem("Rebuild Scene View plugin");
            menuItem.Click += new EventHandler(menuItem_rebuildSceneViewPlugin_Click);
            menu.Items.Add(menuItem);

            menuItem = new ToolStripMenuItem("New C++ component");
            menuItem.Click += new EventHandler(menuItem_newCppComponent_Click);
            menu.Items.Add(menuItem);

            menuItem = new ToolStripMenuItem("New C++ custom asset");
            menuItem.Click += new EventHandler(menuItem_newCppCustomAsset_Click);
            menu.Items.Add(menuItem);

            menuItem = new ToolStripMenuItem("Update Main C++ file");
            menuItem.Click += new EventHandler(menuItem_updateMainCppFile_Click);
            menu.Items.Add(menuItem);

            menu.Show(m_optionsTile, new Point(m_optionsTile.Width, 0));
        }

        private void menuItem_buildSceneViewPlugin_Click(object sender, EventArgs e)
        {
            RebuildEditorPlugin();
        }

        private void menuItem_rebuildSceneViewPlugin_Click(object sender, EventArgs e)
        {
            RebuildEditorPlugin(true);
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

        private void menuItem_newCppCustomAsset_Click(object sender, EventArgs e)
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
                prompt.Title = "Enter asset name:";
                prompt.Value = "UserAsset";
                result = prompt.ShowDialog();
                if (result == DialogResult.OK)
                    CreateNewAsset(dialog.SelectedPath, prompt.Value);
            }
        }

        private void menuItem_updateMainCppFile_Click(object sender, EventArgs e)
        {
            UpdateMainCpp();
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
