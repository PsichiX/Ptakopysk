using System;
using MetroFramework.Controls;
using System.Windows.Forms;

namespace ZasuvkaPtakopyskaExtender.Editors
{
    [PtakopyskPropertyEditor("")]
    public class JsonStringPropertyEditor : PropertyEditor<object>
    {
        private MetroTextBox m_textBox;

        public JsonStringPropertyEditor(object propertyOwner, string propertyName)
            : base(propertyOwner, propertyName, "")
        {
            m_textBox = new MetroTextBox();
            MetroSkinManager.ApplyMetroStyle(m_textBox);
            try { m_textBox.Text = Newtonsoft.Json.JsonConvert.SerializeObject(Value); }
            catch { }
            m_textBox.Width = Width;
            m_textBox.Top = Height;
            m_textBox.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
            m_textBox.TextChanged += new EventHandler(m_textBox_TextChanged);
            Controls.Add(m_textBox);

            Height += m_textBox.Height;
        }

        private void m_textBox_TextChanged(object sender, EventArgs e)
        {
            try { Value = Newtonsoft.Json.JsonConvert.DeserializeObject(m_textBox.Text); }
            catch { }
        }
    }
}
