using System;
using MetroFramework.Controls;

namespace ZasuvkaPtakopyskaExtender.Editors
{
    [PtakopyskPropertyEditor("bool")]
    public class BoolPropertyEditor : PropertyEditor<bool>
    {
        private MetroToggle m_toggle;

        public BoolPropertyEditor(object propertyOwner, string propertyName)
            : base(propertyOwner, propertyName, false)
        {
            m_toggle = new MetroToggle();
            MetroSkinManager.ApplyMetroStyle(m_toggle);
            m_toggle.Checked = Value;
            m_toggle.Top = Height;
            m_toggle.CheckedChanged += new EventHandler(m_toggle_CheckedChanged);
            Controls.Add(m_toggle);

            Height += m_toggle.Height;
        }

        private void m_toggle_CheckedChanged(object sender, EventArgs e)
        {
            Value = m_toggle.Checked;
        }
    }
}
