using MetroFramework.Controls;
using MetroFramework;
using System.Windows.Forms;
using System.Reflection;
using System.Collections.Generic;
using System;

namespace ZasuvkaPtakopyskaExtender.Editors
{
    public abstract class PropertyEditor<T> : MetroUserControl, IEditorJsonValue
    {
        #region Private Data.

        private Dictionary<string, string> m_properties;
        private string m_propertyKey;
        private string m_defaultJson;
        private MetroLabel m_label;

        #endregion



        #region Public Properties.

        public override string Text { get { return m_label.Text; } set { m_label.Text = value; } }
        public IEditorJsonValueChangedCallback EditorJsonValueChangedCallback { get; set; }
        public bool IsProxyEditor { get; set; }
        public string PropertyName { get { return m_propertyKey != null ? m_propertyKey : null; } }
        public string JsonDefaultValue { get { return m_defaultJson; } set { m_defaultJson = value; } }
        public string JsonValue
        {
            get
            {
                if (m_propertyKey != null && m_properties != null && m_properties.ContainsKey(m_propertyKey))
                    return m_properties[m_propertyKey];
                else
                    return JsonDefaultValue;
            }
            set
            {
                if (m_propertyKey != null && m_properties != null)
                {
                    m_properties[m_propertyKey] = value;
                    if (EditorJsonValueChangedCallback != null)
                        EditorJsonValueChangedCallback.OnEditorValueChanged(this, m_propertyKey, value);
                }
            }
        }
        public T DefaultValue
        {
            get
            {
                if (IsProxyEditor)
                    return default(T);
                try { return Newtonsoft.Json.JsonConvert.DeserializeObject<T>(JsonDefaultValue); }
                catch { return default(T); }
            }
            set
            {
                if (IsProxyEditor)
                    return;
                try { JsonDefaultValue = Newtonsoft.Json.JsonConvert.SerializeObject(value); }
                catch { JsonDefaultValue = "null"; }
            }
        }
        public T Value
        {
            get
            {
                if (IsProxyEditor)
                    return DefaultValue;
                try { return Newtonsoft.Json.JsonConvert.DeserializeObject<T>(JsonValue); }
                catch { return DefaultValue; }
            }
            set
            {
                if (IsProxyEditor)
                    return;
                try { JsonValue = Newtonsoft.Json.JsonConvert.SerializeObject(value); }
                catch { JsonValue = JsonDefaultValue; }
            }
        }
        
        #endregion



        #region Construction and Destruction.

        public PropertyEditor(Dictionary<string, string> properties, string propertyName)
        {
            m_properties = properties;
            m_propertyKey = m_properties == null ? null : propertyName;
            DefaultValue = default(T);

            InitializeComponent(propertyName);
        }

        #endregion



        #region Public Functionality.

        public virtual void UpdateEditorValue()
        {
        }

        #endregion



        #region Private Functionality.

        private void InitializeComponent(string label)
        {
            MetroSkinManager.ApplyMetroStyle(this);

            m_label = new MetroLabel();
            MetroSkinManager.ApplyMetroStyle(m_label);
            m_label.FontSize = MetroLabelSize.Small;
            m_label.FontWeight = MetroLabelWeight.Bold;
            if (m_propertyKey != null)
                m_label.Text = m_propertyKey;
            else
                m_label.Text = label;
            m_label.Width = Width;
            m_label.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
            Controls.Add(m_label);

            Height = m_label.Height;
        }

        #endregion
    }
}
