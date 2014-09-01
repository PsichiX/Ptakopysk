using System;
using MetroFramework.Controls;
using System.Windows.Forms;
using System.Reflection;
using System.Collections.Generic;
using System.Globalization;

namespace ZasuvkaPtakopyskaExtender.Editors
{
    [PtakopyskPropertyEditor("@Parsable")]
    [PtakopyskPropertyEditor("char", AliasValueType = "@Parsable<System.SByte>")]
    [PtakopyskPropertyEditor("byte", AliasValueType = "@Parsable<System.Byte>")]
    [PtakopyskPropertyEditor("short", AliasValueType = "@Parsable<System.Int16>")]
    [PtakopyskPropertyEditor("word", AliasValueType = "@Parsable<System.UInt16>")]
    [PtakopyskPropertyEditor("int", AliasValueType = "@Parsable<System.Int32>")]
    [PtakopyskPropertyEditor("dword", AliasValueType = "@Parsable<System.UInt32>")]
    [PtakopyskPropertyEditor("long", AliasValueType = "@Parsable<System.Int64>")]
    [PtakopyskPropertyEditor("qword", AliasValueType = "@Parsable<System.UInt64>")]
    [PtakopyskPropertyEditor("float", AliasValueType = "@Parsable<System.Single>")]
    [PtakopyskPropertyEditor("double", AliasValueType = "@Parsable<System.Double>")]
    public class ParsablePropertyEditor<T> : PropertyEditor<T> where T : IFormattable
    {
        public string StringFormat { get; set; }
        public NumberStyles NumberStyle { get; set; }
        public IFormatProvider FormatProvider { get; set; }

        private MetroTextBox m_textBox;

        public ParsablePropertyEditor(Dictionary<string, string> properties, string propertyName)
            : base(properties, propertyName)
        {
            StringFormat = Settings.DEFAULT_STRING_FORMAT;
            NumberStyle = Settings.DefaultNumberStyle;
            FormatProvider = Settings.DefaultFormatProvider;
            InitializeComponent();
        }

        public override void UpdateEditorValue()
        {
            m_textBox.Text = Value.ToString(StringFormat, FormatProvider);
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
            T v = Value;
            if (Utils.TryParse<T>(m_textBox.Text, out v, v, NumberStyle, FormatProvider))
                Value = v;
        }
    }
}
