using System.Collections.Generic;
using MetroFramework.Controls;
using System.Windows.Forms;
using MetroFramework.Forms;
using System.Drawing;
using System;
using MetroFramework;

namespace ZasuvkaPtakopyskaExtender.Editors
{
    public class CollectionPropertyEditor<T> : PropertyEditor<List<T>>
    {
        private class EditorDialog : MetroForm
        {
            internal class FieldContainer : MetroPanel
            {
                private static readonly int DEFAULT_BUTTON_SIZE = 16;
                private CollectionPropertyEditorUtils.CollectionType m_collectionType;
                private Func<Dictionary<string, string>, string, PropertyEditor<T>> m_creator;
                private MetroTextBox m_memberTextBox;
                private string m_lastMemberName;
                private PropertyEditor<T> m_propertyEditor;
                private Dictionary<string, string> m_imHereForNothing = new Dictionary<string, string>();

                public string MemberName { get { return m_collectionType == CollectionPropertyEditorUtils.CollectionType.JsonObject ? m_memberTextBox.Text : null; } }
                public string LastMemberName { get { return m_lastMemberName; } }
                public PropertyEditor<T> PropertyEditor { get { return m_propertyEditor; } }
                public bool ValidMemberName
                {
                    get { return m_memberTextBox != null && !m_memberTextBox.UseCustomBackColor && !m_memberTextBox.UseCustomForeColor; }
                    set
                    {
                        if (m_memberTextBox != null)
                        {
                            if (value)
                            {
                                m_memberTextBox.UseCustomBackColor = false;
                                m_memberTextBox.UseCustomBackColor = false;
                                m_memberTextBox.FontWeight = MetroTextBoxWeight.Regular;
                            }
                            else
                            {
                                m_memberTextBox.UseCustomBackColor = true;
                                m_memberTextBox.UseCustomBackColor = true;
                                m_memberTextBox.BackColor = Color.Red;
                                m_memberTextBox.ForeColor = Color.White;
                                m_memberTextBox.FontWeight = MetroTextBoxWeight.Bold;
                            }
                            m_memberTextBox.Invalidate();
                        }
                    }
                }

                public FieldContainer(
                    EditorDialog dialog,
                    CollectionPropertyEditorUtils.CollectionType collectionType,
                    Func<Dictionary<string, string>, string, PropertyEditor<T>> creator,
                    string jsonValue,
                    string memberName = null
                    )
                {
                    m_collectionType = collectionType;
                    m_creator = creator;
                    if (m_creator == null)
                        throw new Exception("Creator cannot be null!");
                    MetroSkinManager.ApplyMetroStyle(this);
                    Height = 0;

                    MetroTile remove = new MetroTile();
                    MetroSkinManager.ApplyMetroStyle(remove);
                    remove.Text = "-";
                    remove.TextAlign = ContentAlignment.MiddleCenter;
                    remove.TileTextFontWeight = MetroTileTextWeight.Bold;
                    remove.Width = DEFAULT_BUTTON_SIZE + DEFAULT_BUTTON_SIZE;
                    remove.Height = DEFAULT_BUTTON_SIZE;
                    remove.Left = Width - remove.Width;
                    remove.Anchor = AnchorStyles.Top | AnchorStyles.Right;
                    remove.Click += new EventHandler(remove_Click);
                    Controls.Add(remove);

                    MetroTile add = new MetroTile();
                    MetroSkinManager.ApplyMetroStyle(add);
                    add.Text = "+";
                    add.TextAlign = ContentAlignment.MiddleCenter;
                    add.TileTextFontWeight = MetroTileTextWeight.Bold;
                    add.Width = DEFAULT_BUTTON_SIZE + DEFAULT_BUTTON_SIZE;
                    add.Height = DEFAULT_BUTTON_SIZE;
                    add.Left = Width - add.Width - DEFAULT_SEPARATOR - remove.Width;
                    add.Anchor = AnchorStyles.Top | AnchorStyles.Right;
                    add.Click += new EventHandler(add_Click);
                    Controls.Add(add);

                    Height += Math.Max(add.Height, remove.Height);

                    if (m_collectionType == CollectionPropertyEditorUtils.CollectionType.JsonObject)
                    {
                        m_memberTextBox = new MetroTextBox();
                        MetroSkinManager.ApplyMetroStyle(m_memberTextBox);
                        m_memberTextBox.Text = memberName;
                        m_memberTextBox.Width = Width;
                        m_memberTextBox.Top = Height;
                        m_memberTextBox.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
                        m_memberTextBox.TextChanged += new EventHandler(m_memberTextBox_TextChanged);
                        Controls.Add(m_memberTextBox);
                        Height += m_memberTextBox.Height;
                    }

                    try
                    {
                        m_propertyEditor = m_creator(m_imHereForNothing, "-.-");
                        if (m_propertyEditor == null)
                            throw new Exception("Property editor couldn't be created properly!");
                        MetroUserControl editor = m_propertyEditor as MetroUserControl;
                        IEditorJsonValue jvEditor = m_propertyEditor as IEditorJsonValue;
                        if (editor != null && jvEditor != null)
                        {
                            jvEditor.Text = "Item";
                            jvEditor.JsonValue = jsonValue;
                            jvEditor.UpdateEditorValue();
                            editor.Width = Width;
                            editor.Top = Height;
                            editor.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
                            Controls.Add(editor);
                            Height += editor.Height;
                        }
                    }
                    catch (Exception ex)
                    {
                        while (ex.InnerException != null)
                            ex = ex.InnerException;
                        ErrorPropertyEditor editor = new ErrorPropertyEditor("Item", ex.Message);
                        editor.Tag = string.Format("{0}\n{1}\n\nStack trace:\n{2}", ex.GetType().Name, ex.Message, ex.StackTrace);
                        editor.Width = Width;
                        editor.Top = Height;
                        editor.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
                        Controls.Add(editor);
                        Height += editor.Height;
                    }
                }

                private void m_memberTextBox_TextChanged(object sender, EventArgs e)
                {
                    EditorDialog editorDialog = FindForm() as EditorDialog;
                    if (editorDialog != null)
                        editorDialog.ValidateMemberName(this, m_lastMemberName);
                    m_lastMemberName = m_memberTextBox.Text;
                }

                private void remove_Click(object sender, EventArgs e)
                {
                    EditorDialog editorDialog = FindForm() as EditorDialog;
                    if (editorDialog != null)
                        editorDialog.RemoveItem(this);
                }

                private void add_Click(object sender, EventArgs e)
                {
                    EditorDialog editorDialog = FindForm() as EditorDialog;
                    if (editorDialog != null)
                        editorDialog.AddNewItem(this);
                }
            }

            private static readonly Size DEFAULT_SIZE = new Size(480, 640);
            private static readonly int DEFAULT_BUTTON_HEIGHT = 32;
            private static readonly int DEFAULT_SEPARATOR = 8;

            private CollectionPropertyEditorUtils.CollectionType m_collectionType;
            private Func<Dictionary<string, string>, string, PropertyEditor<T>> m_creator;
            private string m_jsonResult;
            private MetroPanel m_content;
            private MetroScrollBar m_contentScrollbarV;
            private MetroTile m_addButton;
            private MetroTile m_okButton;

            public string JsonString { get { return m_jsonResult; } set { m_jsonResult = value; RebuildList(); } }

            public EditorDialog(
                CollectionPropertyEditorUtils.CollectionType collectionType,
                Func<Dictionary<string, string>, string, PropertyEditor<T>> creator
                )
            {
                m_collectionType = collectionType;
                m_creator = creator;

                MetroSkinManager.ApplyMetroStyle(this);
                Text = "Collection Editor";
                TextAlign = MetroFormTextAlign.Center;
                Size = DEFAULT_SIZE;
                ShowInTaskbar = false;
                ControlBox = false;
                Resizable = false;
                DialogResult = DialogResult.None;

                MetroPanel panel = new MetroPanel();
                MetroSkinManager.ApplyMetroStyle(panel);
                panel.Dock = DockStyle.Fill;
                Controls.Add(panel);

                MetroPanel contentPanel = new MetroPanel();
                MetroSkinManager.ApplyMetroStyle(contentPanel);
                contentPanel.Width = panel.Width;
                contentPanel.Height = panel.Height - DEFAULT_BUTTON_HEIGHT - DEFAULT_SEPARATOR - DEFAULT_BUTTON_HEIGHT - 16;
                contentPanel.Anchor = AnchorStyles.Top | AnchorStyles.Bottom | AnchorStyles.Left | AnchorStyles.Right;
                panel.Controls.Add(contentPanel);

                m_content = new MetroPanel();
                MetroSkinManager.ApplyMetroStyle(m_content);
                m_content.Dock = DockStyle.Fill;
                m_content.Controls.Clear();
                contentPanel.Controls.Add(m_content);

                m_contentScrollbarV = new MetroScrollBar(MetroScrollOrientation.Vertical);
                MetroSkinManager.ApplyMetroStyle(m_contentScrollbarV);
                m_contentScrollbarV.Dock = DockStyle.Right;
                m_contentScrollbarV.Scroll += new ScrollEventHandler(m_contentScrollbarV_Scroll);
                contentPanel.Controls.Add(m_contentScrollbarV);

                m_addButton = new MetroTile();
                MetroSkinManager.ApplyMetroStyle(m_addButton);
                m_addButton.Text = "Add Item";
                m_addButton.TextAlign = ContentAlignment.MiddleCenter;
                m_addButton.TileTextFontWeight = MetroTileTextWeight.Bold;
                m_addButton.Width = panel.Width;
                m_addButton.Height = DEFAULT_BUTTON_HEIGHT;
                m_addButton.Top = panel.Height - DEFAULT_BUTTON_HEIGHT - DEFAULT_SEPARATOR - DEFAULT_BUTTON_HEIGHT;
                m_addButton.Anchor = AnchorStyles.Bottom | AnchorStyles.Left | AnchorStyles.Right;
                m_addButton.Click += new EventHandler(m_addButton_Click);
                panel.Controls.Add(m_addButton);

                m_okButton = new MetroTile();
                MetroSkinManager.ApplyMetroStyle(m_okButton);
                m_okButton.Text = "OK";
                m_okButton.TextAlign = ContentAlignment.MiddleCenter;
                m_okButton.TileTextFontWeight = MetroTileTextWeight.Bold;
                m_okButton.Width = panel.Width;
                m_okButton.Height = DEFAULT_BUTTON_HEIGHT;
                m_okButton.Top = panel.Height - DEFAULT_BUTTON_HEIGHT;
                m_okButton.Anchor = AnchorStyles.Bottom | AnchorStyles.Left | AnchorStyles.Right;
                m_okButton.Click += new EventHandler(m_okButton_Click);
                panel.Controls.Add(m_okButton);
            }

            internal void RemoveItem(FieldContainer field)
            {
                if (m_content.Controls.Contains(field))
                {
                    m_content.Controls.Remove(field);
                    RepositionList();
                }
            }

            internal void AddNewItem(FieldContainer after = null)
            {
                FieldContainer field = new FieldContainer(
                    this,
                    m_collectionType,
                    m_creator,
                    null,
                    m_collectionType == CollectionPropertyEditorUtils.CollectionType.JsonArray ? null : ""
                    );
                field.Width = m_content.Width;
                field.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
                m_content.Controls.Add(field);
                int index = after == null ? -1 : m_content.Controls.GetChildIndex(after);
                if (index >= 0 && index < m_content.Controls.Count - 1)
                    m_content.Controls.SetChildIndex(field, index);
                RepositionList();
            }

            internal void ValidateMemberName(FieldContainer field, string lastName)
            {
                if (field != null)
                {
                    bool duplicated = false;
                    FieldContainer c;
                    foreach (var item in m_content.Controls)
                    {
                        c = item as FieldContainer;
                        if (c != null && c != field)
                        {
                            if (c.MemberName == field.MemberName)
                            {
                                duplicated = true;
                                c.ValidMemberName = false;
                            }
                            if (!c.ValidMemberName && c.LastMemberName == lastName)
                                c.ValidMemberName = true;
                        }
                    }
                    field.ValidMemberName = !duplicated;
                }
            }

            private void RebuildList()
            {
                m_content.Controls.Clear();
                if (string.IsNullOrEmpty(m_jsonResult))
                    return;

                try
                {
                    Newtonsoft.Json.Linq.JContainer collection = Newtonsoft.Json.JsonConvert.DeserializeObject<Newtonsoft.Json.Linq.JContainer>(m_jsonResult);

                    if (collection.Type == Newtonsoft.Json.Linq.JTokenType.Array && m_collectionType == CollectionPropertyEditorUtils.CollectionType.JsonArray)
                    {
                        Newtonsoft.Json.Linq.JArray arr = collection as Newtonsoft.Json.Linq.JArray;
                        if (arr != null)
                        {
                            FieldContainer field;
                            foreach (var item in arr)
                            {
                                field = new FieldContainer(this, m_collectionType, m_creator, item.ToString(Newtonsoft.Json.Formatting.None));
                                field.Width = m_content.Width;
                                field.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
                                m_content.Controls.Add(field);
                            }
                        }
                    }
                    else if (collection.Type == Newtonsoft.Json.Linq.JTokenType.Object && m_collectionType == CollectionPropertyEditorUtils.CollectionType.JsonObject)
                    {
                        Newtonsoft.Json.Linq.JObject obj = collection as Newtonsoft.Json.Linq.JObject;
                        if (obj != null)
                        {
                            FieldContainer field;
                            foreach (var item in obj.Properties())
                            {
                                field = new FieldContainer(this, m_collectionType, m_creator, item.Value.ToString(Newtonsoft.Json.Formatting.None), item.Name);
                                field.Width = m_content.Width;
                                field.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
                                m_content.Controls.Add(field);
                            }
                        }
                    }
                }
                catch { }
                RepositionList();
            }

            private void RepositionList()
            {
                int y = 0;
                foreach (Control item in m_content.Controls)
                {
                    item.Width = m_content.Width;
                    item.Top = y;
                    y += item.Height + DEFAULT_SEPARATOR;
                }
                m_contentScrollbarV.Minimum = 1;
                m_contentScrollbarV.Maximum = y;
                m_contentScrollbarV.LargeChange = m_content.Height;
                m_contentScrollbarV.Visible = m_contentScrollbarV.Maximum > m_contentScrollbarV.LargeChange;
                m_content.VerticalScroll.Minimum = m_contentScrollbarV.Minimum;
                m_content.VerticalScroll.Maximum = m_contentScrollbarV.Maximum;
                m_content.VerticalScroll.LargeChange = m_contentScrollbarV.LargeChange;
                m_content.Height = y;
            }

            private void m_contentScrollbarV_Scroll(object sender, ScrollEventArgs e)
            {
                m_content.VerticalScroll.Value = m_contentScrollbarV.Value;
            }

            private void m_addButton_Click(object sender, EventArgs e)
            {
                AddNewItem();
            }

            private void m_okButton_Click(object sender, EventArgs e)
            {
                if (m_collectionType == CollectionPropertyEditorUtils.CollectionType.JsonArray)
                {
                    string json = "[ ";
                    FieldContainer field;
                    int c = m_content.Controls.Count;
                    string v;
                    for (int i = 0; i < c; ++i)
                    {
                        field = m_content.Controls[i] as FieldContainer;
                        if (field != null)
                        {
                            v = field.PropertyEditor.JsonValue;
                            if (!string.IsNullOrEmpty(v))
                            {
                                json += v;
                                if (i < c - 1)
                                    json += ", ";
                            }
                        }
                    }
                    json += " ]";
                    JsonString = json;
                    DialogResult = DialogResult.OK;
                }
                else if (m_collectionType == CollectionPropertyEditorUtils.CollectionType.JsonObject)
                {
                    string json = "{ ";
                    FieldContainer field;
                    int c = m_content.Controls.Count;
                    string v;
                    for (int i = 0; i < c; ++i)
                    {
                        field = m_content.Controls[i] as FieldContainer;
                        if (field != null)
                        {
                            v = field.PropertyEditor.JsonValue;
                            if (!string.IsNullOrEmpty(field.MemberName) && !string.IsNullOrEmpty(v))
                            {
                                json += "\"" + field.MemberName + "\": " + v;
                                if (i < c - 1)
                                    json += ", ";
                            }
                        }
                    }
                    json += " }";
                    m_jsonResult = json;
                    DialogResult = DialogResult.OK;
                }
                Close();
            }
        }

        public CollectionPropertyEditorUtils.CollectionType CollectionType { get { return m_collectonType; } }

        private CollectionPropertyEditorUtils.CollectionType m_collectonType;
        private Func<Dictionary<string, string>, string, PropertyEditor<T>> m_creator;
        private MetroButton m_exploreButton;

        public CollectionPropertyEditor(
            Dictionary<string, string> properties,
            string propertyName,
            CollectionPropertyEditorUtils.CollectionType collectionType,
            Func<Dictionary<string, string>, string, PropertyEditor<T>> creator
            )
            : base(properties, propertyName)
        {
            m_collectonType = collectionType;
            m_creator = creator;
            if (m_creator == null)
                throw new Exception("Creator function cannot be null!");
            InitializeComponent();
        }

        public override void UpdateEditorValue()
        {
        }

        private void InitializeComponent()
        {
            IsProxyEditor = true;

            m_exploreButton = new MetroButton();
            MetroSkinManager.ApplyMetroStyle(m_exploreButton);
            m_exploreButton.Text = "Explore Items";
            m_exploreButton.Width = Width;
            m_exploreButton.Top = Height;
            m_exploreButton.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
            m_exploreButton.Click += new System.EventHandler(m_exploreButton_Click);
            Controls.Add(m_exploreButton);

            Height = m_exploreButton.Bottom;
        }

        private void m_exploreButton_Click(object sender, System.EventArgs e)
        {
            EditorDialog dialog = new EditorDialog(m_collectonType, m_creator);
            dialog.JsonString = JsonValue;
            DialogResult result = dialog.ShowDialog();
            if (result == DialogResult.OK)
                JsonValue = dialog.JsonString;
        }
    }
}
