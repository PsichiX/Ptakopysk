using MetroFramework.Controls;
using MetroFramework;
using System.Windows.Forms;
using System.Reflection;
using System.Collections.Generic;

namespace ZasuvkaPtakopyskaExtender.Editors
{
    public abstract class PropertyEditor<T> : MetroUserControl, IEditorJsonValue
    {
        #region Private Data.

        private object m_object;
        private PropertyInfo m_property;
        private Dictionary<string, object> m_propertiesContainer;
        private string m_propertyKey;
        private T m_default;
        private MetroLabel m_label;

        #endregion



        #region Public Properties.

        public T DefaultValue { get { return m_default; } set { m_default = value; } }
        public T Value
        {
            get
            {
                if (m_property != null && m_object != null && m_property.CanRead)
                    return (T)m_property.GetValue(m_object, null);
                else if (m_propertyKey != null && m_propertiesContainer != null && m_propertiesContainer.ContainsKey(m_propertyKey) && m_propertiesContainer[m_propertyKey] is T)
                    return (T)m_propertiesContainer[m_propertyKey];
                else
                    return m_default;
            }
            set
            {
                if (m_property != null && m_object != null && m_property.CanWrite)
                    m_property.SetValue(m_object, value, null);
                else if (m_propertyKey != null && m_propertiesContainer != null)
                    m_propertiesContainer[m_propertyKey] = value;
            }
        }
        public string JsonDefaultValue
        {
            get
            {
                try { return Newtonsoft.Json.JsonConvert.SerializeObject(DefaultValue); }
                catch { return ""; }
            }
            set
            {
                try { DefaultValue = string.IsNullOrEmpty(value) ? default(T) : Newtonsoft.Json.JsonConvert.DeserializeObject<T>(value); }
                catch { DefaultValue = default(T); }
            }
        }
        public string JsonValue
        {
            get
            {
                try { return Newtonsoft.Json.JsonConvert.SerializeObject(Value); }
                catch { return ""; }
            }
            set
            {
                try { Value = string.IsNullOrEmpty(value) ? DefaultValue : Newtonsoft.Json.JsonConvert.DeserializeObject<T>(value); }
                catch { Value = DefaultValue; }
            }
        }
        public bool IsPropertyBound { get { return m_property != null; } }
        public bool IsPropertyKeyBound { get { return m_propertyKey != null; } }

        #endregion



        #region Construction and Destruction.

        public PropertyEditor(object propertyOwner, string propertyName, T defaultValue)
        {
            m_object = propertyOwner;
            m_property = m_object == null ? null : propertyOwner.GetType().GetProperty(propertyName);
            m_default = defaultValue;

            InitializeComponent(propertyName);
        }

        public PropertyEditor(Dictionary<string, object> properties, string propertyName, T defaultValue)
        {
            m_propertiesContainer = properties;
            m_propertyKey = m_propertiesContainer == null ? null : propertyName;
            m_default = defaultValue;

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
            if (m_property != null)
                m_label.Text = m_property.Name;
            else if (m_propertyKey != null)
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
