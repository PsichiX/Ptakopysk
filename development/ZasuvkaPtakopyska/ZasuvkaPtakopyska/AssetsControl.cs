using MetroFramework;
using MetroFramework.Controls;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;
using ZasuvkaPtakopyskaExtender;
using ZasuvkaPtakopyskaExtender.Editors;

namespace ZasuvkaPtakopyska
{
    public class AssetsControl : MetroPanel, IEditorJsonValueChangedCallback, MetroSidePanel.IMetroSidePanelScrollableContent
    {
        #region Nested Classes.

        public class Asset_PropertyEditor : PropertyEditor<object>, IEditorJsonValueChangedCallback
        {
            public struct MetaEditorInfo
            {
                public string property;
                public Type editorType;

                public MetaEditorInfo(string property, Type editorType)
                {
                    this.property = property;
                    this.editorType = editorType;
                }
            }

            public class Tags_PropertyEditor : CollectionPropertyEditor<string>
            {
                public Tags_PropertyEditor(Dictionary<string, string> properties, string propertyName)
                    : base(
                    properties,
                    propertyName,
                    CollectionPropertyEditorUtils.CollectionType.JsonArray,
                    (pd, pn) => new String_PropertyEditor(pd, pn)
                    )
                {
                }
            }

            private SceneViewPlugin.AssetType m_assetType;
            private string m_lastId;
            private String_PropertyEditor m_idEditor;
            private List<KeyValuePair<string, IEditorJsonValue>> m_metaEditors = new List<KeyValuePair<string, IEditorJsonValue>>();
            private Tags_PropertyEditor m_tagsEditor;
            private Dictionary<string, string> m_subProperties = new Dictionary<string, string>();

            public Asset_PropertyEditor(Dictionary<string, string> properties, string propertyName, SceneViewPlugin.AssetType assetType, MetaEditorInfo[] metaEditors, string id, string meta, string tags)
                : base(properties, propertyName)
            {
                m_assetType = assetType;
                m_lastId = id;
                InitializeComponent(metaEditors, id, meta, tags);
            }

            public T GetMetaEditor<T>(string name) where T : class, IEditorJsonValue
            {
                foreach (var editor in m_metaEditors)
                    if (editor.Key == name && editor.Value is T)
                        return editor.Value as T;
                return null;
            }

            public override void UpdateEditorValue()
            {
                try
                {
                    Newtonsoft.Json.Linq.JObject data = Newtonsoft.Json.JsonConvert.DeserializeObject<Newtonsoft.Json.Linq.JObject>(JsonValue);
                    if (data != null)
                    {
                        Newtonsoft.Json.Linq.JToken v;
                        if (data.TryGetValue("id", out v))
                        {
                            m_subProperties["id"] = v.ToString(Newtonsoft.Json.Formatting.None);
                            m_idEditor.UpdateEditorValue();
                        }
                        foreach (var metaEditor in m_metaEditors)
                        {
                            if (data.TryGetValue(metaEditor.Key, out v))
                            {
                                m_subProperties[metaEditor.Key] = v.ToString(Newtonsoft.Json.Formatting.None);
                                metaEditor.Value.UpdateEditorValue();
                            }
                        }
                        if (data.TryGetValue("tags", out v))
                        {
                            m_subProperties["tags"] = v.ToString(Newtonsoft.Json.Formatting.None);
                            m_tagsEditor.UpdateEditorValue();
                        }
                    }
                }
                catch { }
            }

            public void OnEditorValueChanged(IEditorJsonValue editor, string property, string jsonValue)
            {
                m_subProperties[property] = jsonValue;
                UpdateJsonValue();
            }

            private void InitializeComponent(MetaEditorInfo[] metaEditors, string id, string meta, string tags)
            {
                IsProxyEditor = true;

                m_subProperties["id"] = Newtonsoft.Json.JsonConvert.SerializeObject(id);
                m_subProperties["tags"] = string.IsNullOrEmpty(tags) ? "null" : Newtonsoft.Json.JsonConvert.SerializeObject(tags.Split('|'));

                MetroButton applyButton = new MetroButton();
                MetroSkinManager.ApplyMetroStyle(applyButton);
                applyButton.Text = "Apply";
                applyButton.Width = Width - 20;
                applyButton.Top = Height;
                applyButton.Left = 10;
                applyButton.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
                applyButton.Click += new EventHandler(applyButton_Click);
                Controls.Add(applyButton);
                Height += applyButton.Height + DEFAULT_SEPARATOR;

                MetroButton removeButton = new MetroButton();
                MetroSkinManager.ApplyMetroStyle(removeButton);
                removeButton.Text = "Remove";
                removeButton.Width = Width - 20;
                removeButton.Top = Height;
                removeButton.Left = 10;
                removeButton.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
                removeButton.Click += new EventHandler(removeButton_Click);
                Controls.Add(removeButton);
                Height += removeButton.Height + DEFAULT_SEPARATOR;

                m_idEditor = new String_PropertyEditor(m_subProperties, "id");
                m_idEditor.Width = Width - 20;
                m_idEditor.Top = Height;
                m_idEditor.Left = 10;
                m_idEditor.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
                Controls.Add(m_idEditor);
                m_idEditor.EditorJsonValueChangedCallback = this;
                Height += m_idEditor.Height + DEFAULT_SEPARATOR;

                if (metaEditors != null && metaEditors.Length > 0)
                {
                    string[] metaParts = string.IsNullOrEmpty(meta) ? null : meta.Split('|');
                    int metaPartsIndex = 0;
                    foreach (MetaEditorInfo info in metaEditors)
                    {
                        if (metaParts != null && metaPartsIndex < metaParts.Length)
                            m_subProperties[info.property] = Newtonsoft.Json.JsonConvert.SerializeObject(metaParts[metaPartsIndex]);
                        metaPartsIndex++;
                        try
                        {
                            object obj = Activator.CreateInstance(info.editorType, m_subProperties, info.property);
                            MetroUserControl editor = obj as MetroUserControl;
                            IEditorJsonValue jvEditor = obj as IEditorJsonValue;
                            if (editor != null && jvEditor != null)
                            {
                                jvEditor.Text = info.property;
                                jvEditor.UpdateEditorValue();
                                editor.Top = Height;
                                editor.Width = Width - 20;
                                editor.Left = 10;
                                editor.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
                                Controls.Add(editor);
                                jvEditor.EditorJsonValueChangedCallback = this;
                                m_metaEditors.Add(new KeyValuePair<string, IEditorJsonValue>(info.property, jvEditor));
                                Height += editor.Height + DEFAULT_SEPARATOR;
                            }
                        }
                        catch (Exception ex)
                        {
                            while (ex.InnerException != null)
                                ex = ex.InnerException;
                            ErrorPropertyEditor editor = new ErrorPropertyEditor(info.property, ex.Message);
                            editor.Tag = string.Format("{0}\n{1}\n\nStack trace:\n{2}", ex.GetType().Name, ex.Message, ex.StackTrace);
                            editor.Top = Height;
                            editor.Width = Width - 20;
                            editor.Left = 10;
                            editor.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
                            Controls.Add(editor);
                            Height += editor.Height + DEFAULT_SEPARATOR;
                        }
                    }
                }

                m_tagsEditor = new Tags_PropertyEditor(m_subProperties, "tags");
                m_tagsEditor.Width = Width - 20;
                m_tagsEditor.Top = Height;
                m_tagsEditor.Left = 10;
                m_tagsEditor.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
                Controls.Add(m_tagsEditor);
                m_tagsEditor.EditorJsonValueChangedCallback = this;
                Height += m_tagsEditor.Height;

                UpdateJsonValue();
            }

            private void UpdateJsonValue()
            {
                if (!m_subProperties.ContainsKey("id") || !m_subProperties.ContainsKey("tags"))
                    return;
                foreach (var metaEditor in m_metaEditors)
                    if (!m_subProperties.ContainsKey(metaEditor.Key))
                        return;

                string json = "{ \"id\": " + m_subProperties["id"];
                foreach (var metaEditor in m_metaEditors)
                    json += ", \"" + metaEditor.Key + "\": " + m_subProperties[metaEditor.Key];
                json += ", \"tags\": " + m_subProperties["tags"] + " }";
                JsonValue = json;
            }

            private void applyButton_Click(object sender, EventArgs e)
            {
                string freeQuery = "\"free\": [ \"" + m_lastId + "\" ], ";
                string query = "{ " + (string.IsNullOrEmpty(m_lastId) ? "" : freeQuery) + "\"load\": [ " + JsonValue + " ] }";
                if (SceneViewPlugin.QueryAssets(m_assetType, query) != null)
                {
                    MainForm mainForm = FindForm() as MainForm;
                    if (mainForm != null)
                    {
                        mainForm.ExploreAssetsProperties(m_assetType);
                        mainForm.RefreshSceneView();
                    }
                }
            }

            private void removeButton_Click(object sender, EventArgs e)
            {
                string query = "{ \"free\": [ \"" + m_lastId + "\" ] }";
                if (SceneViewPlugin.QueryAssets(m_assetType, query) != null)
                {
                    MainForm mainForm = FindForm() as MainForm;
                    if (mainForm != null)
                    {
                        mainForm.ExploreAssetsProperties(m_assetType);
                        mainForm.RefreshSceneView();
                    }
                }
            }
        }

        #endregion



        #region Private Static Data.

        private static readonly int DEFAULT_SEPARATOR = 8;

        #endregion



        #region Private Data.

        private SceneViewPlugin.AssetType m_type;
        private Dictionary<string, string> m_properties = new Dictionary<string, string>();
        private MetroButton m_addAssetButton;

        #endregion



        #region Public Properties.

        public MetroSidePanel SidePanel { get; set; }
        public Rectangle ScrollableContentRectangle
        {
            get
            {
                Rectangle rect;
                this.CalculateContentsRectangle(out rect);
                return rect;
            }
        }
        public int VerticalScrollValue { get { return VerticalScroll.Value; } set { VerticalScroll.Value = value; } }
        public int VerticalScrollMaximum { get { return VerticalScroll.Maximum; } set { VerticalScroll.Maximum = value; } }
        public int VerticalScrollLargeChange { get { return VerticalScroll.LargeChange; } set { VerticalScroll.LargeChange = value; } }
        public int HorizontalScrollValue { get { return HorizontalScroll.Value; } set { HorizontalScroll.Value = value; } }
        public int HorizontalScrollMaximum { get { return HorizontalScroll.Maximum; } set { HorizontalScroll.Maximum = value; } }
        public int HorizontalScrollLargeChange { get { return HorizontalScroll.LargeChange; } set { HorizontalScroll.LargeChange = value; } }

        #endregion



        #region Construction and Destruction.

        public AssetsControl(SceneViewPlugin.AssetType type, ProjectModel projectModel)
        {
            m_type = type;

            MetroSkinManager.ApplyMetroStyle(this);

            int y = DEFAULT_SEPARATOR;
            y = InitializeAssets(projectModel, y);
            Height = y;
        }

        #endregion



        #region Public Functionality.

        public void OnEditorValueChanged(IEditorJsonValue editor, string property, string jsonValue)
        {
        }

        #endregion



        #region Private Functionality.

        private int InitializeAssets(ProjectModel projectModel, int y)
        {
            Asset_PropertyEditor temp;
            List<string> assets = SceneViewPlugin.ListAssets(m_type);
            var data = SceneViewPlugin.GetAssetsInfo(m_type, assets);
            foreach (var d in data)
                y = AddAsset(d.id, d.meta, d.tags == null ? "" : string.Join("|", d.tags.ToArray()), projectModel, out temp, y);
            
            m_addAssetButton = new MetroButton();
            MetroSkinManager.ApplyMetroStyle(m_addAssetButton);
            m_addAssetButton.Text = "Add New Asset";
            m_addAssetButton.FontWeight = MetroButtonWeight.Bold;
            m_addAssetButton.Top = y;
            m_addAssetButton.Width = Width;
            m_addAssetButton.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
            m_addAssetButton.Click += new EventHandler(addAssetButton_Click);
            Controls.Add(m_addAssetButton);
            y = m_addAssetButton.Bottom;

            return y;
        }

        private int AddAsset(string id, string meta, string tags, ProjectModel projectModel, out Asset_PropertyEditor outEditor, int y)
        {
            m_properties[id] = "null";
            Asset_PropertyEditor editor = null;
            if (m_type == SceneViewPlugin.AssetType.Texture)
            {
                editor = new Asset_PropertyEditor(m_properties, id, m_type,
                    new Asset_PropertyEditor.MetaEditorInfo[] {
                        new Asset_PropertyEditor.MetaEditorInfo("path", typeof(Path_PropertyEditor))
                    }, id, meta, tags);
                Path_PropertyEditor pathEditor = editor == null ? null : editor.GetMetaEditor<Path_PropertyEditor>("path");
                if (pathEditor != null)
                {
                    pathEditor.FileFilter = "Texture files (*.png)|*.png";
                    if (projectModel != null)
                        pathEditor.RootPath = projectModel.WorkingDirectory + @"\" + projectModel.ActiveTargetWorkingDirectory;
                }
            }
            else if (m_type == SceneViewPlugin.AssetType.Shader)
            {
                editor = new Asset_PropertyEditor(m_properties, id, m_type,
                    new Asset_PropertyEditor.MetaEditorInfo[] {
                        new Asset_PropertyEditor.MetaEditorInfo("vspath", typeof(Path_PropertyEditor)),
                        new Asset_PropertyEditor.MetaEditorInfo("fspath", typeof(Path_PropertyEditor))
                    }, id, meta, tags);
                Path_PropertyEditor vspathEditor = editor == null ? null : editor.GetMetaEditor<Path_PropertyEditor>("vspath");
                if (vspathEditor != null)
                {
                    vspathEditor.FileFilter = "Vertex shader files (*.vs)|*.vs";
                    if (projectModel != null)
                        vspathEditor.RootPath = projectModel.WorkingDirectory + @"\" + projectModel.ActiveTargetWorkingDirectory;
                }
                Path_PropertyEditor fspathEditor = editor == null ? null : editor.GetMetaEditor<Path_PropertyEditor>("fspath");
                if (fspathEditor != null)
                {
                    fspathEditor.FileFilter = "Fragment shader files (*.fs)|*.fs";
                    if (projectModel != null)
                        fspathEditor.RootPath = projectModel.WorkingDirectory + @"\" + projectModel.ActiveTargetWorkingDirectory;
                }
            }
            else if (m_type == SceneViewPlugin.AssetType.Sound)
            {
                editor = new Asset_PropertyEditor(m_properties, id, m_type,
                    new Asset_PropertyEditor.MetaEditorInfo[] {
                        new Asset_PropertyEditor.MetaEditorInfo("path", typeof(Path_PropertyEditor))
                    }, id, meta, tags);
                Path_PropertyEditor pathEditor = editor == null ? null : editor.GetMetaEditor<Path_PropertyEditor>("path");
                if (pathEditor != null)
                {
                    pathEditor.FileFilter = "Sound files (*.mp3)|*.mp3";
                    if (projectModel != null)
                        pathEditor.RootPath = projectModel.WorkingDirectory + @"\" + projectModel.ActiveTargetWorkingDirectory;
                }
            }
            else if (m_type == SceneViewPlugin.AssetType.Music)
            {
                editor = new Asset_PropertyEditor(m_properties, id, m_type,
                    new Asset_PropertyEditor.MetaEditorInfo[] {
                        new Asset_PropertyEditor.MetaEditorInfo("path", typeof(Path_PropertyEditor))
                    }, id, meta, tags);
                Path_PropertyEditor pathEditor = editor == null ? null : editor.GetMetaEditor<Path_PropertyEditor>("path");
                if (pathEditor != null)
                {
                    pathEditor.FileFilter = "Music files (*.mp3)|*.mp3";
                    if (projectModel != null)
                        pathEditor.RootPath = projectModel.WorkingDirectory + @"\" + projectModel.ActiveTargetWorkingDirectory;
                }
            }
            else if (m_type == SceneViewPlugin.AssetType.Font)
            {
                editor = new Asset_PropertyEditor(m_properties, id, m_type,
                    new Asset_PropertyEditor.MetaEditorInfo[] {
                        new Asset_PropertyEditor.MetaEditorInfo("path", typeof(Path_PropertyEditor))
                    }, id, meta, tags);
                Path_PropertyEditor pathEditor = editor == null ? null : editor.GetMetaEditor<Path_PropertyEditor>("path");
                if (pathEditor != null)
                {
                    pathEditor.FileFilter = "Font files (*.ttf)|*.ttf";
                    if (projectModel != null)
                        pathEditor.RootPath = projectModel.WorkingDirectory + @"\" + projectModel.ActiveTargetWorkingDirectory;
                }
            }
            else if (m_type == SceneViewPlugin.AssetType.CustomAsset)
            {
                editor = new Asset_PropertyEditor(m_properties, id, m_type,
                    new Asset_PropertyEditor.MetaEditorInfo[] {
                        new Asset_PropertyEditor.MetaEditorInfo("type", typeof(EnumPropertyEditor)),
                        new Asset_PropertyEditor.MetaEditorInfo("path", typeof(Path_PropertyEditor))
                    }, id, meta, tags);
                EnumPropertyEditor typeEditor = editor == null ? null : editor.GetMetaEditor<EnumPropertyEditor>("type");
                if (typeEditor != null)
                {
                    List<string> values = SceneViewPlugin.ListCustomAssets();
                    typeEditor.ValuesSource = values == null ? null : values.ToArray();
                }
                Path_PropertyEditor pathEditor = editor == null ? null : editor.GetMetaEditor<Path_PropertyEditor>("path");
                if (pathEditor != null)
                {
                    pathEditor.FileFilter = "Custom asset files (*.*)|*.*";
                    if (projectModel != null)
                        pathEditor.RootPath = projectModel.WorkingDirectory + @"\" + projectModel.ActiveTargetWorkingDirectory;
                }
            }
            if (editor != null)
            {
                editor.Text = id;
                editor.UpdateEditorValue();
                editor.Top = y;
                editor.Width = Width;
                editor.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
                Controls.Add(editor);
                editor.EditorJsonValueChangedCallback = this;
                y = editor.Bottom;
            }
            outEditor = editor;
            return y + DEFAULT_SEPARATOR + DEFAULT_SEPARATOR;
        }

        #endregion



        #region Private Events Handlers.

        private void addAssetButton_Click(object sender, EventArgs e)
        {
            if (m_addAssetButton == null)
                return;

            MainForm mainForm = FindForm() as MainForm;
            if (mainForm == null)
                return;

            int y = m_addAssetButton.Top;
            Controls.Remove(m_addAssetButton);
            m_addAssetButton = null;
            Asset_PropertyEditor editor;
            y = AddAsset("", "", "", mainForm.ProjectModel == null ? null : mainForm.ProjectModel, out editor, y);
            Height = y;
            if (editor != null)
            {
                editor.Text = "<New Asset>";
                if (SidePanel != null)
                {
                    SidePanel.UpdateScrollbars();
                    SidePanel.ScrollbarV.Value = editor.Bottom;
                }
            }
        }

        #endregion
    }
}
