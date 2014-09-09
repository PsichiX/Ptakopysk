using MetroFramework.Controls;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Windows.Forms;
using ZasuvkaPtakopyskaExtender;
using ZasuvkaPtakopyskaExtender.Editors;

namespace ZasuvkaPtakopyska
{
    public class ConfigControl : MetroPanel, IEditorJsonValueChangedCallback, MetroSidePanel.IMetroSidePanelScrollableContent
    {
        #region Public Nested Classes.

        public class Config
        {
            public class Window
            {
                public class VideoMode
                {
                    public uint width = 0;
                    public uint height = 0;
                }

                public List<int> color = new List<int>();
                public VideoMode videoMode = new VideoMode();
                public string name = "";
                public List<string> style = new List<string>();
            }

            public class LifeCycle
            {
                public float fixedFps = 0.0f;
                public float fixedStep = 0.0f;
            }

            public Window window = new Window();
            public LifeCycle lifeCycle = new LifeCycle();
            public Dictionary<string, string> scenes = new Dictionary<string, string>();
        }

        public class ArrayStylePropertyEditor : CollectionPropertyEditor<string>
        {
            private static readonly string[] STYLES = new string[] { "Titlebar", "Resize", "Close", "Fullscreen", "Default" };

            public ArrayStylePropertyEditor(Dictionary<string, string> properties, string propertyName)
                : base(
                properties,
                propertyName,
                CollectionPropertyEditorUtils.CollectionType.JsonArray,
                (pd, pn) => new EnumPropertyEditor(pd, pn, STYLES)
                )
            {
            }
        }

        public class StringMapPathPropertyEditor : CollectionPropertyEditor<string>
        {
            public StringMapPathPropertyEditor(Dictionary<string, string> properties, string propertyName, string rootPath)
                : base(
                properties,
                propertyName,
                CollectionPropertyEditorUtils.CollectionType.JsonObject,
                (pd, pn) => {
                    Path_PropertyEditor editor = new Path_PropertyEditor(pd, pn);
                    editor.FileFilter = ScenePageControl.DEFAULT_SCENE_FILTER;
                    editor.RootPath = rootPath;
                    return editor;
                })
            {
            }
        }

        #endregion



        #region Private Static Data.

        private static readonly int DEFAULT_SEPARATOR = 8;
        private static readonly string DEFAULT_CONFIG_FILTER = "Ptakopysk game config file (*.json)|*.json";

        #endregion



        #region Private Data.

        private string m_path;
        private MetroButton m_reloadButton;
        private MetroButton m_saveButton;
        private MetroButton m_saveAsButton;
        private Color_PropertyEditor m_windowColor;
        private ParsablePropertyEditor<uint> m_windowWidth;
        private ParsablePropertyEditor<uint> m_windowHeight;
        private String_PropertyEditor m_windowName;
        private ArrayStylePropertyEditor m_windowStyles;
        private ParsablePropertyEditor<float> m_lifeCycleFixedFps;
        private ParsablePropertyEditor<float> m_lifeCycleFixedStep;
        private StringMapPathPropertyEditor m_scenes;
        private Dictionary<string, string> m_properties = new Dictionary<string, string>();

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

        public ConfigControl(string path, ProjectModel model)
        {
            m_path = path;
            string json = File.ReadAllText(m_path);
            ConfigControl.Config config = null;
            try { config = Newtonsoft.Json.JsonConvert.DeserializeObject<ConfigControl.Config>(json); }
            catch { config = new Config(); }

            MetroSkinManager.ApplyMetroStyle(this);

            m_properties["windowColor"] = Newtonsoft.Json.JsonConvert.SerializeObject(config.window.color);
            m_properties["windowWidth"] = Newtonsoft.Json.JsonConvert.SerializeObject(config.window.videoMode.width);
            m_properties["windowHeight"] = Newtonsoft.Json.JsonConvert.SerializeObject(config.window.videoMode.height);
            m_properties["windowStyles"] = Newtonsoft.Json.JsonConvert.SerializeObject(config.window.style);
            m_properties["windowName"] = Newtonsoft.Json.JsonConvert.SerializeObject(config.window.name);
            m_properties["lifeCycleFixedFps"] = Newtonsoft.Json.JsonConvert.SerializeObject(config.lifeCycle.fixedFps);
            m_properties["lifeCycleFixedStep"] = Newtonsoft.Json.JsonConvert.SerializeObject(config.lifeCycle.fixedStep);
            m_properties["scenes"] = Newtonsoft.Json.JsonConvert.SerializeObject(config.scenes);

            int y = DEFAULT_SEPARATOR;

            m_reloadButton = new MetroButton();
            MetroSkinManager.ApplyMetroStyle(m_reloadButton);
            m_reloadButton.Text = "Reload config";
            m_reloadButton.Width = Width;
            m_reloadButton.Top = y;
            m_reloadButton.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
            m_reloadButton.Click += m_reloadButton_Click;
            Controls.Add(m_reloadButton);
            y = m_reloadButton.Bottom + DEFAULT_SEPARATOR;

            m_saveButton = new MetroButton();
            MetroSkinManager.ApplyMetroStyle(m_saveButton);
            m_saveButton.Text = "Save config";
            m_saveButton.Width = Width;
            m_saveButton.Top = y;
            m_saveButton.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
            m_saveButton.Click += m_saveButton_Click;
            Controls.Add(m_saveButton);
            y = m_saveButton.Bottom + DEFAULT_SEPARATOR;

            m_saveAsButton = new MetroButton();
            MetroSkinManager.ApplyMetroStyle(m_saveAsButton);
            m_saveAsButton.Text = "Save config as...";
            m_saveAsButton.Width = Width;
            m_saveAsButton.Top = y;
            m_saveAsButton.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
            m_saveAsButton.Click += m_saveAsButton_Click;
            Controls.Add(m_saveAsButton);
            y = m_saveAsButton.Bottom + DEFAULT_SEPARATOR;

            m_windowColor = new Color_PropertyEditor(m_properties, "windowColor");
            m_windowColor.Text = "Window Color";
            m_windowColor.Width = Width;
            m_windowColor.Top = y;
            m_windowColor.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
            Controls.Add(m_windowColor);
            m_windowColor.EditorJsonValueChangedCallback = this;
            y = m_windowColor.Bottom + DEFAULT_SEPARATOR;

            m_windowWidth = new ParsablePropertyEditor<uint>(m_properties, "windowWidth");
            m_windowWidth.Text = "Window Width";
            m_windowWidth.Width = Width;
            m_windowWidth.Top = y;
            m_windowWidth.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
            Controls.Add(m_windowWidth);
            m_windowWidth.EditorJsonValueChangedCallback = this;
            y = m_windowWidth.Bottom + DEFAULT_SEPARATOR;

            m_windowHeight = new ParsablePropertyEditor<uint>(m_properties, "windowHeight");
            m_windowHeight.Text = "Window Height";
            m_windowHeight.Width = Width;
            m_windowHeight.Top = y;
            m_windowHeight.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
            Controls.Add(m_windowHeight);
            m_windowHeight.EditorJsonValueChangedCallback = this;
            y = m_windowHeight.Bottom + DEFAULT_SEPARATOR;

            m_windowName = new String_PropertyEditor(m_properties, "windowName");
            m_windowName.Text = "Window Name";
            m_windowName.Width = Width;
            m_windowName.Top = y;
            m_windowName.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
            Controls.Add(m_windowName);
            m_windowName.EditorJsonValueChangedCallback = this;
            y = m_windowName.Bottom + DEFAULT_SEPARATOR;

            m_windowStyles = new ArrayStylePropertyEditor(m_properties, "windowStyles");
            m_windowStyles.Text = "Window Styles";
            m_windowStyles.Width = Width;
            m_windowStyles.Top = y;
            m_windowStyles.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
            Controls.Add(m_windowStyles);
            m_windowStyles.EditorJsonValueChangedCallback = this;
            y = m_windowStyles.Bottom + DEFAULT_SEPARATOR;

            m_lifeCycleFixedFps = new ParsablePropertyEditor<float>(m_properties, "lifeCycleFixedFps");
            m_lifeCycleFixedFps.Text = "Life Cycle Fixed Fps";
            m_lifeCycleFixedFps.Width = Width;
            m_lifeCycleFixedFps.Top = y;
            m_lifeCycleFixedFps.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
            Controls.Add(m_lifeCycleFixedFps);
            m_lifeCycleFixedFps.EditorJsonValueChangedCallback = this;
            y = m_lifeCycleFixedFps.Bottom + DEFAULT_SEPARATOR;

            m_lifeCycleFixedStep = new ParsablePropertyEditor<float>(m_properties, "lifeCycleFixedStep");
            m_lifeCycleFixedStep.Text = "Life Cycle Fixed Step";
            m_lifeCycleFixedStep.Width = Width;
            m_lifeCycleFixedStep.Top = y;
            m_lifeCycleFixedStep.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
            Controls.Add(m_lifeCycleFixedStep);
            m_lifeCycleFixedStep.EditorJsonValueChangedCallback = this;
            y = m_lifeCycleFixedStep.Bottom + DEFAULT_SEPARATOR;

            m_scenes = new StringMapPathPropertyEditor(m_properties, "scenes", model == null ? null : model.WorkingDirectory + @"\" + model.ActiveTargetWorkingDirectory);
            m_scenes.Text = "Scenes";
            m_scenes.Width = Width;
            m_scenes.Top = y;
            m_scenes.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
            Controls.Add(m_scenes);
            m_scenes.EditorJsonValueChangedCallback = this;
            y = m_scenes.Bottom + DEFAULT_SEPARATOR;

            m_windowColor.UpdateEditorValue();
            m_windowWidth.UpdateEditorValue();
            m_windowHeight.UpdateEditorValue();
            m_windowName.UpdateEditorValue();
            m_windowStyles.UpdateEditorValue();
            m_lifeCycleFixedFps.UpdateEditorValue();
            m_lifeCycleFixedStep.UpdateEditorValue();
            m_scenes.UpdateEditorValue();
        }

        #endregion



        #region Public Functionality.

        public void OnEditorValueChanged(IEditorJsonValue editor, string property, string jsonValue)
        {
            if (property == "lifeCycleFixedFps")
            {
                float v = m_lifeCycleFixedFps.Value;
                bool irc = m_lifeCycleFixedStep.IsRaisingEditorJsonValueChangedCallback;
                m_lifeCycleFixedStep.IsRaisingEditorJsonValueChangedCallback = false;
                m_lifeCycleFixedStep.Value = v > 0.0f ? 1.0f / v : 0.0f;
                m_lifeCycleFixedStep.UpdateEditorValue();
                m_lifeCycleFixedStep.IsRaisingEditorJsonValueChangedCallback = irc;
            }
            else if (property == "lifeCycleFixedStep")
            {
                float v = m_lifeCycleFixedStep.Value;
                bool irc = m_lifeCycleFixedFps.IsRaisingEditorJsonValueChangedCallback;
                m_lifeCycleFixedFps.IsRaisingEditorJsonValueChangedCallback = false;
                m_lifeCycleFixedFps.Value = v > 0.0f ? 1.0f / v : 0.0f;
                m_lifeCycleFixedFps.UpdateEditorValue();
                m_lifeCycleFixedFps.IsRaisingEditorJsonValueChangedCallback = irc;
            }
        }

        public Config ToConfig()
        {
            Config config = new Config();
            config.window.color = m_windowColor.Value;
            config.window.videoMode.width = m_windowWidth.Value;
            config.window.videoMode.height = m_windowHeight.Value;
            config.window.name = m_windowName.Value;
            try { config.window.style = Newtonsoft.Json.JsonConvert.DeserializeObject<List<string>>(m_windowStyles.JsonValue); }
            catch { }
            config.lifeCycle.fixedFps = m_lifeCycleFixedFps.Value;
            config.lifeCycle.fixedStep = m_lifeCycleFixedStep.Value;
            try { config.scenes = Newtonsoft.Json.JsonConvert.DeserializeObject<Dictionary<string, string>>(m_scenes.JsonValue); }
            catch { }
            return config;
        }

        public string ToJson(Newtonsoft.Json.Formatting formatting = Newtonsoft.Json.Formatting.Indented)
        {
            Config config = ToConfig();
            try { return Newtonsoft.Json.JsonConvert.SerializeObject(config, formatting); }
            catch { return null; }
        }

        #endregion



        #region Provate Event Handlers.

        private void m_reloadButton_Click(object sender, System.EventArgs e)
        {
            MainForm mainForm = FindForm() as MainForm;
            if (mainForm != null)
                mainForm.ExploreConfigProperties(m_path);
        }

        private void m_saveButton_Click(object sender, System.EventArgs e)
        {
            string json = ToJson();
            if (string.IsNullOrEmpty(json))
            {
                MetroFramework.MetroMessageBox.Show(FindForm(), "Error during serialization to JSON!", "Config Serialization Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }
            File.WriteAllText(m_path, json);
            MetroFramework.MetroMessageBox.Show(FindForm(), "Path: " + m_path, "Config Successfully Saved", MessageBoxButtons.OK, MessageBoxIcon.Information);
        }

        private void m_saveAsButton_Click(object sender, System.EventArgs e)
        {
            SaveFileDialog dialog = new SaveFileDialog();
            dialog.InitialDirectory = Path.GetDirectoryName(m_path);
            dialog.FileName = Path.GetFileName(m_path);
            dialog.RestoreDirectory = true;
            dialog.OverwritePrompt = true;
            dialog.Filter = DEFAULT_CONFIG_FILTER;
            DialogResult result = dialog.ShowDialog();
            if (result == DialogResult.OK)
            {
                string json = ToJson();
                if (string.IsNullOrEmpty(json))
                {
                    MetroFramework.MetroMessageBox.Show(FindForm(), "Error during serialization to JSON!", "Config Serialization Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return;
                }
                m_path = dialog.FileName;
                File.WriteAllText(m_path, json);
                MetroFramework.MetroMessageBox.Show(FindForm(), "Path: " + m_path, "Config Successfully Saved", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }

        #endregion
    }
}
