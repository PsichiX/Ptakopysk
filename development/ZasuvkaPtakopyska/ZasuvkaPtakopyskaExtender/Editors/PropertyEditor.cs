using MetroFramework.Controls;
using MetroFramework;
using System.Windows.Forms;
using System.Reflection;

namespace ZasuvkaPtakopyskaExtender.Editors
{
    public abstract class PropertyEditor<T> : MetroUserControl
    {
        #region Private Data.

        private object m_object;
        private PropertyInfo m_property;
        private T m_default;
        private MetroLabel m_label;

        #endregion



        #region Public Properties.

        public T Value
        {
            get
            {
                if (m_property != null && m_object != null && m_property.CanRead)
                    return (T)m_property.GetValue(m_object, null);
                else
                    return m_default;
            }
            set
            {
                if (m_property != null && m_object != null && m_property.CanWrite)
                    m_property.SetValue(m_object, value, null);
            }
        }
        public T DefaultValue { get { return m_default; } set { m_default = value; } }

        #endregion



        #region Construction and Destruction.

        public PropertyEditor(object propertyOwner, string propertyName, T defaultValue)
        {
            MetroSkinManager.ApplyMetroStyle(this);
            
            m_object = propertyOwner;
            m_property = m_object == null ? null : propertyOwner.GetType().GetProperty(propertyName);
            m_default = defaultValue;

            m_label = new MetroLabel();
            MetroSkinManager.ApplyMetroStyle(m_label);
            m_label.FontSize = MetroLabelSize.Small;
            m_label.FontWeight = MetroLabelWeight.Bold;
            if (m_property != null)
                m_label.Text = m_property.Name;
            m_label.Width = Width;
            m_label.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
            Controls.Add(m_label);

            Height = m_label.Height;
        }

        #endregion
    }
}
