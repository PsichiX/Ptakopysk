using System;
using MetroFramework.Controls;
using ZasuvkaPtakopyskaExtender;
using ZasuvkaPtakopyskaExtender.Editors;
using System.Windows.Forms;
using PtakopyskMetaGenerator;
using System.Collections.Generic;
using MetroFramework;
using System.Drawing;

namespace ZasuvkaPtakopyska
{
    public class GameObjectPropertiesControl : MetroPanel, IEditorJsonValueChangedCallback, MetroSidePanel.IMetroSidePanelScrollableContent
    {
        #region Private Static Data.

        private static readonly int DEFAULT_SEPARATOR = 8;

        #endregion



        #region Private Data.

        private int m_goHandle = 0;
        private bool m_goIsPrefab = false;
        private Dictionary<string, string> m_goData;
        private List<IEditorJsonValue> m_editors = new List<IEditorJsonValue>();

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

        public GameObjectPropertiesControl(int handle, bool isPrefab)
        {
            if (handle == 0)
                throw new ArgumentException("Game Object handle cannot be 0!");

            m_goHandle = handle;
            m_goIsPrefab = isPrefab;
            m_goData = PtakopyskInterface.Instance.QueryGameObject(handle, isPrefab, "{ \"get\": null }");

            MetroSkinManager.ApplyMetroStyle(this);

            int y = DEFAULT_SEPARATOR;
            y = InitializePrefabSection(y);
            y = InitializePropertiesSection(y);
            y = InitializeComponentsSection(y);
            Height = y;
        }

        #endregion



        #region Public Functionality.

        public void OnEditorValueChanged(IEditorJsonValue editor, string property, string jsonValue)
        {
            string query = PrepareSetQuery(property, jsonValue);
            Dictionary<string, string> result = PtakopyskInterface.Instance.QueryGameObject(m_goHandle, m_goIsPrefab, query);
            m_goData.Clear();
            foreach (string key in result.Keys)
                m_goData.Add(key, result[key]);
            foreach (IEditorJsonValue e in m_editors)
                if (e.PropertyName != property && result.ContainsKey(e.PropertyName))
                    e.UpdateEditorValue();
            MainForm mainForm = FindForm() as MainForm;
            if (mainForm != null)
            {
                if (result != null)
                    mainForm.RefreshSceneView();
                if (property == "properties/Id")
                    mainForm.DoAction(new MainForm.Action("GameObjectIdChanged", m_goHandle));
            }
        }

        public void UpdateEditorsValues()
        {
            Dictionary<string, string> result = PtakopyskInterface.Instance.QueryGameObject(m_goHandle, m_goIsPrefab, "{ \"get\": null }");
            m_goData.Clear();
            foreach (string key in result.Keys)
                m_goData.Add(key, result[key]);
            foreach (IEditorJsonValue e in m_editors)
                if (result.ContainsKey(e.PropertyName))
                    e.UpdateEditorValue();
            MainForm mainForm = FindForm() as MainForm;
            if (mainForm != null)
                mainForm.RefreshSceneView();
        }

        #endregion



        #region Private Functionality.

        private int InitializePrefabSection(int y)
        {
            string pref = null;
            try
            {
                if (m_goData.ContainsKey("prefab"))
                    pref = Newtonsoft.Json.JsonConvert.DeserializeObject<string>(m_goData["prefab"]);
            }
            catch { }
            if (string.IsNullOrEmpty(pref))
                return y;

            MetroLabel label = new MetroLabel();
            MetroSkinManager.ApplyMetroStyle(label);
            label.Text = "Prefab: " + pref;
            label.FontWeight = MetroLabelWeight.Bold;
            label.FontSize = MetroLabelSize.Small;
            label.Top = y;
            label.Width = Width;
            label.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
            Controls.Add(label);
            y = label.Bottom;

            MetroButton selectPrefabButton = new MetroButton();
            MetroSkinManager.ApplyMetroStyle(selectPrefabButton);
            selectPrefabButton.Text = "Select Prefab";
            selectPrefabButton.Top = y;
            selectPrefabButton.Width = Width;
            selectPrefabButton.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
            selectPrefabButton.Click += new EventHandler(selectPrefabButton_Click);
            selectPrefabButton.Tag = pref;
            Controls.Add(selectPrefabButton);
            y = selectPrefabButton.Bottom + DEFAULT_SEPARATOR;

            MetroButton applyToPrefabButton = new MetroButton();
            MetroSkinManager.ApplyMetroStyle(applyToPrefabButton);
            applyToPrefabButton.Text = "Apply to Prefab";
            applyToPrefabButton.Top = y;
            applyToPrefabButton.Width = Width;
            applyToPrefabButton.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
            applyToPrefabButton.Click += new EventHandler(applyToPrefabButton_Click);
            applyToPrefabButton.Tag = pref;
            Controls.Add(applyToPrefabButton);
            y = applyToPrefabButton.Bottom + DEFAULT_SEPARATOR;

            return y;
        }

        private int InitializePropertiesSection(int y)
        {
            String_PropertyEditor idEditor = new String_PropertyEditor(m_goData, "properties/Id");
            idEditor.Text = "Id";
            idEditor.UpdateEditorValue();
            idEditor.Top = y;
            idEditor.Width = Width;
            idEditor.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
            Controls.Add(idEditor);
            idEditor.EditorJsonValueChangedCallback = this;
            m_editors.Add(idEditor);
            y = idEditor.Bottom + DEFAULT_SEPARATOR;

            Bool_PropertyEditor activeEditor = new Bool_PropertyEditor(m_goData, "properties/Active");
            activeEditor.Text = "Active";
            activeEditor.UpdateEditorValue();
            activeEditor.Top = y;
            activeEditor.Width = Width;
            activeEditor.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
            Controls.Add(activeEditor);
            activeEditor.EditorJsonValueChangedCallback = this;
            m_editors.Add(activeEditor);
            y = activeEditor.Bottom + DEFAULT_SEPARATOR;

            Int_PropertyEditor orderEditor = new Int_PropertyEditor(m_goData, "properties/Order");
            orderEditor.Text = "Order";
            orderEditor.UpdateEditorValue();
            orderEditor.Top = y;
            orderEditor.Width = Width;
            orderEditor.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
            Controls.Add(orderEditor);
            orderEditor.EditorJsonValueChangedCallback = this;
            m_editors.Add(orderEditor);
            y = orderEditor.Bottom + DEFAULT_SEPARATOR;

            JsonStringPropertyEditor metaDataEditor = new JsonStringPropertyEditor(m_goData, "properties/MetaData");
            metaDataEditor.Text = "MetaData";
            metaDataEditor.UpdateEditorValue();
            metaDataEditor.Top = y;
            metaDataEditor.Width = Width;
            metaDataEditor.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
            Controls.Add(metaDataEditor);
            metaDataEditor.EditorJsonValueChangedCallback = this;
            m_editors.Add(metaDataEditor);
            y = metaDataEditor.Bottom + DEFAULT_SEPARATOR;

            return y;
        }

        private int InitializeComponentsSection(int y)
        {
            if (m_goData == null)
                return y;

            List<string> components = new List<string>();
            string[] parts;
            foreach (string p in m_goData.Keys)
            {
                parts = p.Split('/');
                if (parts != null && parts.Length > 1 && parts[0] == "components" && !components.Contains(parts[1]))
                    components.Add(parts[1]);
            }
            foreach (string c in components)
                y = InitializeComponentFragment(c, y);

            MetroButton addComponentButton = new MetroButton();
            MetroSkinManager.ApplyMetroStyle(addComponentButton);
            addComponentButton.Text = "Add New Component";
            addComponentButton.FontWeight = MetroButtonWeight.Bold;
            addComponentButton.Top = y + DEFAULT_SEPARATOR + DEFAULT_SEPARATOR + DEFAULT_SEPARATOR;
            addComponentButton.Width = Width;
            addComponentButton.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
            addComponentButton.Click += new EventHandler(addComponentButton_Click);
            Controls.Add(addComponentButton);
            y = addComponentButton.Bottom + DEFAULT_SEPARATOR;

            return y;
        }

        private int InitializeComponentFragment(string component, int y)
        {
            if (string.IsNullOrEmpty(component))
                return y;

            MetaComponent meta = MetaComponentsManager.Instance.FindMetaComponent(component);
            if (meta == null)
                return y;

            MetroButton componentButton = new MetroButton();
            MetroSkinManager.ApplyMetroStyle(componentButton);
            componentButton.Text = component;
            componentButton.FontWeight = MetroButtonWeight.Bold;
            componentButton.Top = y + DEFAULT_SEPARATOR + DEFAULT_SEPARATOR + DEFAULT_SEPARATOR;
            componentButton.Width = Width;
            componentButton.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
            componentButton.Click += new EventHandler(componentButton_Click);
            Controls.Add(componentButton);
            y = componentButton.Bottom + DEFAULT_SEPARATOR;

            if (meta.FunctionalityTriggers != null && meta.FunctionalityTriggers.Count > 0)
            {
                MetroLabel label = new MetroLabel();
                MetroSkinManager.ApplyMetroStyle(label);
                label.Text = "Functionality Triggers";
                label.FontWeight = MetroLabelWeight.Bold;
                label.FontSize = MetroLabelSize.Small;
                label.Top = y;
                label.Width = Width;
                label.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
                Controls.Add(label);
                y = label.Bottom;

                foreach (string func in meta.FunctionalityTriggers)
                {
                    MetroButton btn = new MetroButton();
                    MetroSkinManager.ApplyMetroStyle(btn);
                    btn.Tag = meta.Name;
                    btn.Text = func;
                    btn.Top = y;
                    btn.Width = Width - 20;
                    btn.Left = 10;
                    btn.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
                    btn.Click += new EventHandler(triggerFunctionality_Click);
                    Controls.Add(btn);
                    y = btn.Bottom + DEFAULT_SEPARATOR;
                }
            }

            List<MetaProperty> props = MetaComponentsManager.Instance.GetFlattenPropertiesOf(meta);
            if (props == null || props.Count == 0)
                return y;

            foreach (MetaProperty p in props)
            {
                Type t = PropertyEditorsManager.Instance.FindPropertyEditorByValueType(p.ValueType);
                if (t == null)
                {
                    string msg = string.Format("Property editor for type: \"{0}\" not found!", p.ValueType);
                    ErrorPropertyEditor editor = new ErrorPropertyEditor(p.Name, msg);
                    editor.UpdateEditorValue();
                    editor.Top = y;
                    editor.Width = Width;
                    editor.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
                    Controls.Add(editor);
                    y = editor.Bottom + DEFAULT_SEPARATOR;
                }
                else
                    y = InitializePropertyFragment(component, p, t, y);
            }

            return y;
        }

        private int InitializePropertyFragment(string component, MetaProperty property, Type editorType, int y)
        {
            if (property == null || editorType == null)
                return y;

            string propertyPath = "components/" + component + "/" + property.Name;
            if (m_goData == null || !m_goData.ContainsKey(propertyPath))
                return y;

            try
            {
                object obj = Activator.CreateInstance(editorType, m_goData, propertyPath);
                MetroUserControl editor = obj as MetroUserControl;
                IEditorJsonValue jvEditor = obj as IEditorJsonValue;
                if (editor != null && jvEditor != null)
                {
                    jvEditor.Text = property.Name;
                    jvEditor.JsonDefaultValue = property.DefaultValue;
                    jvEditor.UpdateEditorValue();
                    editor.Top = y;
                    editor.Width = Width;
                    editor.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
                    Controls.Add(editor);
                    jvEditor.EditorJsonValueChangedCallback = this;
                    m_editors.Add(jvEditor);
                    y = editor.Bottom + DEFAULT_SEPARATOR;
                }
            }
            catch (Exception ex)
            {
                while (ex.InnerException != null)
                    ex = ex.InnerException;
                ErrorPropertyEditor editor = new ErrorPropertyEditor(property.Name, ex.Message);
                editor.Tag = string.Format("{0}\n{1}\n\nStack trace:\n{2}", ex.GetType().Name, ex.Message, ex.StackTrace);
                editor.Top = y;
                editor.Width = Width;
                editor.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
                Controls.Add(editor);
                y = editor.Bottom + DEFAULT_SEPARATOR;
            }

            return y;
        }

        private string PrepareSetQuery(string property, string jsonValue)
        {
            if (m_goData == null || string.IsNullOrEmpty(property))
                return null;

            string[] parts = property.Split('/');
            if (parts != null && parts.Length > 0)
            {
                if (parts[0] == "properties" && parts.Length > 1)
                    return "{ \"set\": { \"properties\": { \"" + parts[1] + "\": " + jsonValue + " } }, \"get\": null }";
                else if (parts[0] == "components" && parts.Length > 2)
                    return "{ \"set\": { \"components\": { \"" + parts[1] + "\": { \"" + parts[2] + "\": " + jsonValue + " } } }, \"get\": null }";
            }
            return null;
        }

        #endregion



        #region Private Events Handlers.

        private void selectPrefabButton_Click(object sender, EventArgs e)
        {
            MainForm mainForm = FindForm() as MainForm;
            MetroButton button = sender as MetroButton;
            if (mainForm != null && button != null && button.Tag is string)
            {
                int handle = PtakopyskInterface.Instance.FindGameObjectHandleById(button.Tag as string, true, 0);
                if (handle != 0)
                    mainForm.ExploreGameObjectProperties(handle, true);
            }
        }

        private void applyToPrefabButton_Click(object sender, EventArgs e)
        {
            MainForm mainForm = FindForm() as MainForm;
            MetroButton button = sender as MetroButton;
            if (mainForm != null && button != null && button.Tag is string)
            {
                int handle = PtakopyskInterface.Instance.FindGameObjectHandleById(button.Tag as string, true, 0);
                if (handle != 0)
                    PtakopyskInterface.Instance.DuplicateGameObject(m_goHandle, m_goIsPrefab, handle, true);
            }
        }

        private void componentButton_Click(object sender, EventArgs e)
        {
            MetroButton btn = sender as MetroButton;
            if (btn == null)
                return;

            MetroContextMenu menu = new MetroContextMenu(null);
            MetroSkinManager.ApplyMetroStyle(menu);
            ToolStripMenuItem menuItem;

            menuItem = new ToolStripMenuItem("Remove Component");
            menuItem.Tag = btn.Text;
            menuItem.Click += new EventHandler(menuItem_removeComponent_Click);
            menu.Items.Add(menuItem);

            menu.Show(btn, new Point(0, btn.Height));
        }

        private void triggerFunctionality_Click(object sender, EventArgs e)
        {
            MetroButton btn = sender as MetroButton;
            if (btn == null || !(btn.Tag is string))
                return;

            if (PtakopyskInterface.Instance.TriggerGameObjectComponentFunctionality(m_goHandle, m_goIsPrefab, btn.Tag as string, btn.Text))
                UpdateEditorsValues();
        }

        private void addComponentButton_Click(object sender, EventArgs e)
        {
            MetroButton btn = sender as MetroButton;
            if (btn == null)
                return;

            List<string> components = PtakopyskInterface.Instance.GetComponentsIds();
            if (components == null || components.Count == 0)
                return;

            MetroContextMenu menu = new MetroContextMenu(null);
            MetroSkinManager.ApplyMetroStyle(menu);
            ToolStripMenuItem menuItem;

            foreach (string c in components)
            {
                menuItem = new ToolStripMenuItem(c);
                menuItem.Click += new EventHandler(menuItem_addNewComponent_Click);
                menu.Items.Add(menuItem);
            }

            menu.Show(btn, new Point(0, btn.Height));
        }

        private void menuItem_removeComponent_Click(object sender, EventArgs e)
        {
            ToolStripMenuItem menuItem = sender as ToolStripMenuItem;
            if (menuItem == null || !(menuItem.Tag is string))
                return;

            string query = "{ \"set\": { \"components\": { \"" + (menuItem.Tag as string) + "\": null } } }";
            Dictionary<string, string> result = PtakopyskInterface.Instance.QueryGameObject(m_goHandle, m_goIsPrefab, query);
            MainForm mainForm = FindForm() as MainForm;
            if (mainForm != null)
            {
                mainForm.ExploreGameObjectProperties(m_goHandle, m_goIsPrefab);
                mainForm.RefreshSceneView();
            }
        }

        private void menuItem_addNewComponent_Click(object sender, EventArgs e)
        {
            ToolStripMenuItem menuItem = sender as ToolStripMenuItem;
            if (menuItem == null)
                return;

            string query = "{ \"set\": { \"components\": { \"" + menuItem.Text + "\": 0 } } }";
            Dictionary<string, string> result = PtakopyskInterface.Instance.QueryGameObject(m_goHandle, m_goIsPrefab, query);
            MainForm mainForm = FindForm() as MainForm;
            if (mainForm != null)
            {
                mainForm.ExploreGameObjectProperties(m_goHandle, m_goIsPrefab);
                mainForm.RefreshSceneView();
            }
        }

        #endregion
    }
}
