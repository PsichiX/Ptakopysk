using System.Collections.Generic;
using MetroFramework.Controls;
using System.Windows.Forms;
using System;
using System.Drawing;
using MetroFramework;

namespace ZasuvkaPtakopyskaExtender.Editors
{
    public class ErrorPropertyEditor : PropertyEditor<string>
    {
        private MetroTextBox m_textBox;

        public ErrorPropertyEditor(string propertyName, string errorMessage)
            : base(null, propertyName, "")
        {
            InitializeComponent(errorMessage);
        }

        private void InitializeComponent(string message)
        {
            m_textBox = new MetroTextBox();
            MetroSkinManager.ApplyMetroStyle(m_textBox);
            m_textBox.ReadOnly = true;
            m_textBox.Text = message;
            m_textBox.Width = Width;
            m_textBox.Top = Height;
            m_textBox.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
            m_textBox.UseCustomBackColor = true;
            m_textBox.UseCustomForeColor = true;
            m_textBox.BackColor = Color.Red;
            m_textBox.ForeColor = Color.White;
            Controls.Add(m_textBox);

            Height += m_textBox.Height;
        }
    }
}
