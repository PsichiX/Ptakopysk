using System;
using MetroFramework.Controls;
using System.Windows.Forms;
using System.Reflection;

namespace ZasuvkaPtakopyskaExtender.Editors
{
    [PtakopyskPropertyEditor("string")]
    [PtakopyskPropertyEditor("std::string")]
    public class StringPropertyEditor : PropertyEditor<string>
    {
        private MetroTextBox m_textBox;

        public StringPropertyEditor(object propertyOwner, string propertyName)
            : base(propertyOwner, propertyName, "")
        {
            m_textBox = new MetroTextBox();
            MetroSkinManager.ApplyMetroStyle(m_textBox);
            m_textBox.Text = Value;
            m_textBox.Width = Width;
            m_textBox.Top = Height;
            m_textBox.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
            m_textBox.TextChanged += new EventHandler(m_textBox_TextChanged);
            Controls.Add(m_textBox);

            Height += m_textBox.Height;
        }

        private void m_textBox_TextChanged(object sender, EventArgs e)
        {
            Value = m_textBox.Text;
        }
    }
}
