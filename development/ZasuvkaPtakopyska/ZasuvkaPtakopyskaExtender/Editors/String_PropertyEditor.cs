using System;
using MetroFramework.Controls;
using System.Windows.Forms;
using System.Collections.Generic;

namespace ZasuvkaPtakopyskaExtender.Editors
{
    [PtakopyskPropertyEditor("std::string")]
    [PtakopyskPropertyEditor("string", TypePriority = 1)]
    [PtakopyskPropertyEditor("sf::String")]
    [PtakopyskPropertyEditor("String", TypePriority = 1)]
    public class String_PropertyEditor : PropertyEditor<string>
    {
        private MetroTextBox m_textBox;

        public String_PropertyEditor(Dictionary<string, string> properties, string propertyName)
            : base(properties, propertyName)
        {
            InitializeComponent();
        }

        public override void UpdateEditorValue()
        {
            m_textBox.Text = Value;
        }

        private void InitializeComponent()
        {
            m_textBox = new MetroTextBox();
            MetroSkinManager.ApplyMetroStyle(m_textBox);
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
