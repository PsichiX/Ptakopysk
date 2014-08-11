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
    public class GameObjectPropertiesEditor : MetroPanel, IEditorJsonValueChangedCallback, MetroSidePanel.IMetroSidePanelScrollableContent
    {
        #region Private Static Data.

        private static readonly int DEFAULT_SEPARATOR = 8;

        #endregion



        #region Private Data.

        private int m_goHandle = 0;
        private bool m_goIsPrefab = false;
        private Dictionary<string, string> m_goData;

        #endregion



        #region Public Properties.

        public Rectangle ScrollableContentRectangle
        {
            get
            {
                Rectangle rect;
                this.CalculateContentsRectangle(out rect);
                return rect;
            }
        }
        public bool IsAffectedByScroll { get { return true; } }
        public int ScrollValue { get { return VerticalScroll.Value; } set { VerticalScroll.Value = value; } }
        public int ScrollMaximum { get { return VerticalScroll.Maximum; } set { VerticalScroll.Maximum = value; } }
        public int ScrollLargeChange { get { return VerticalScroll.LargeChange; } set { VerticalScroll.LargeChange = value; } }

        #endregion



        #region Construction and Destruction.

        public GameObjectPropertiesEditor(int handle, bool isPrefab)
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
            MainForm mainForm = FindForm() as MainForm;
            if (mainForm != null)
            {
                if (result != null)
                    mainForm.RefreshSceneView();
                if (property == "properties/Id")
                    mainForm.DoAction(new MainForm.Action("GameObjectIdChanged", m_goHandle));
            }
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
            y = idEditor.Bottom + DEFAULT_SEPARATOR;

            Bool_PropertyEditor activeEditor = new Bool_PropertyEditor(m_goData, "properties/Active");
            activeEditor.Text = "Active";
            activeEditor.UpdateEditorValue();
            activeEditor.Top = y;
            activeEditor.Width = Width;
            activeEditor.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
            Controls.Add(activeEditor);
            activeEditor.EditorJsonValueChangedCallback = this;
            y = activeEditor.Bottom + DEFAULT_SEPARATOR;

            Int_PropertyEditor orderEditor = new Int_PropertyEditor(m_goData, "properties/Order");
            orderEditor.Text = "Order";
            orderEditor.UpdateEditorValue();
            orderEditor.Top = y;
            orderEditor.Width = Width;
            orderEditor.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
            Controls.Add(orderEditor);
            orderEditor.EditorJsonValueChangedCallback = this;
            y = orderEditor.Bottom + DEFAULT_SEPARATOR;

            JsonStringPropertyEditor metaDataEditor = new JsonStringPropertyEditor(m_goData, "properties/MetaData");
            metaDataEditor.Text = "MetaData";
            metaDataEditor.UpdateEditorValue();
            metaDataEditor.Top = y;
            metaDataEditor.Width = Width;
            metaDataEditor.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
            Controls.Add(metaDataEditor);
            metaDataEditor.EditorJsonValueChangedCallback = this;
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

            return y;
        }

        private int InitializeComponentFragment(string component, int y)
        {
            if (string.IsNullOrEmpty(component))
                return y;

            MetaComponent meta = MetaComponentsManager.Instance.FindMetaComponent(component);
            if (meta == null)
                return y;

            MetroButton btn = new MetroButton();
            MetroSkinManager.ApplyMetroStyle(btn);
            btn.Text = component;
            btn.FontWeight = MetroButtonWeight.Bold;
            btn.Top = y + DEFAULT_SEPARATOR + DEFAULT_SEPARATOR + DEFAULT_SEPARATOR;
            btn.Width = Width;
            btn.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
            Controls.Add(btn);
            y = btn.Bottom + DEFAULT_SEPARATOR;

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
            MetroButton selectPrefabButton = sender as MetroButton;
            if (mainForm != null && selectPrefabButton != null && selectPrefabButton.Tag is string)
            {
                int handle = PtakopyskInterface.Instance.FindGameObjectHandleById(selectPrefabButton.Tag as string, true, 0);
                if (handle != 0)
                    mainForm.ExploreGameObjectProperties(handle, true);
            }
        }

        #endregion
    }
}
