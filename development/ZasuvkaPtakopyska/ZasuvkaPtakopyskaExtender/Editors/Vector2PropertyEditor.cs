using System;
using MetroFramework.Controls;
using System.Windows.Forms;
using System.Collections.Generic;
using System.Globalization;

namespace ZasuvkaPtakopyskaExtender.Editors
{
    [PtakopyskPropertyEditor("@Vector2")]
    [PtakopyskPropertyEditor("char[2]", AliasValueType = "@Vector2<System.SByte>")]
    [PtakopyskPropertyEditor("byte[2]", AliasValueType = "@Vector2<System.Byte>")]
    [PtakopyskPropertyEditor("short[2]", AliasValueType = "@Vector2<System.Int16>")]
    [PtakopyskPropertyEditor("word[2]", AliasValueType = "@Vector2<System.UInt16>")]
    [PtakopyskPropertyEditor("int[2]", AliasValueType = "@Vector2<System.Int32>")]
    [PtakopyskPropertyEditor("dword[2]", AliasValueType = "@Vector2<System.UInt32>")]
    [PtakopyskPropertyEditor("long[2]", AliasValueType = "@Vector2<System.Int64>")]
    [PtakopyskPropertyEditor("qword[2]", AliasValueType = "@Vector2<System.UInt64>")]
    [PtakopyskPropertyEditor("float[2]", AliasValueType = "@Vector2<System.Single>")]
    [PtakopyskPropertyEditor("double[2]", AliasValueType = "@Vector2<System.Double>")]
    [PtakopyskPropertyEditor("b2Vec2", AliasValueType = "@Vector2<System.Single>")]
    [PtakopyskPropertyEditor("sf::Vector2i", AliasValueType = "@Vector2<System.Int32>")]
    [PtakopyskPropertyEditor("sf::Vector2f", AliasValueType = "@Vector2<System.Single>")]
    [PtakopyskPropertyEditor("sf::Vector2<int>", AliasValueType = "@Vector2<System.Int32>")]
    [PtakopyskPropertyEditor("sf::Vector2<float>", AliasValueType = "@Vector2<System.Single>")]
    [PtakopyskPropertyEditor("Vector2i", AliasValueType = "@Vector2<System.Int32>", TypePriority = 1)]
    [PtakopyskPropertyEditor("Vector2f", AliasValueType = "@Vector2<System.Single>", TypePriority = 1)]
    [PtakopyskPropertyEditor("Vector2<int>", AliasValueType = "@Vector2<System.Int32>", TypePriority = 1)]
    [PtakopyskPropertyEditor("Vector2<float>", AliasValueType = "@Vector2<System.Single>", TypePriority = 1)]
    public class Vector2PropertyEditor<T> : PropertyEditor<List<T>> where T : IFormattable
    {
        private static readonly int DEFAULT_LABEL_WIDTH = 30;

        public string StringFormat { get; set; }
        public NumberStyles NumberStyle { get; set; }
        public IFormatProvider FormatProvider { get; set; }

        private MetroTextBox m_xTextBox;
        private MetroTextBox m_yTextBox;

        public Vector2PropertyEditor(Dictionary<string, string> properties, string propertyName)
            : base(properties, propertyName)
        {
            StringFormat = Settings.DEFAULT_STRING_FORMAT;
            NumberStyle = Settings.DefaultNumberStyle;
            FormatProvider = Settings.DefaultFormatProvider;
            InitializeComponent();
        }

        public override void UpdateEditorValue()
        {
            if (!ValidateValue())
                return;

            var ov = Value;
            m_xTextBox.Text = ov[0].ToString(StringFormat, FormatProvider);
            m_yTextBox.Text = ov[1].ToString(StringFormat, FormatProvider);
        }

        private bool ValidateValue()
        {
            var ov = Value;
            var dv = DefaultValue;
            if (ov == null)
            {
                Value = dv == null || dv.Count < 2 ? new List<T>(new T[] { default(T), default(T) }) : new List<T>(dv);
                ov = Value;
            }
            return ov != null && ov.Count >= 2;
        }

        private void InitializeComponent()
        {
            MetroLabel label;

            label = new MetroLabel();
            MetroSkinManager.ApplyMetroStyle(label);
            label.Text = "X:";
            label.Width = DEFAULT_LABEL_WIDTH;
            label.Top = Height;
            label.Anchor = AnchorStyles.Top | AnchorStyles.Left;
            Controls.Add(label);

            m_xTextBox = new MetroTextBox();
            MetroSkinManager.ApplyMetroStyle(m_xTextBox);
            m_xTextBox.Width = Width - label.Width;
            m_xTextBox.Top = Height;
            m_xTextBox.Left = label.Width;
            m_xTextBox.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
            m_xTextBox.TextChanged += new EventHandler(m_xTextBox_TextChanged);
            Controls.Add(m_xTextBox);

            Height += m_xTextBox.Height;

            label = new MetroLabel();
            MetroSkinManager.ApplyMetroStyle(label);
            label.Text = "Y:";
            label.Width = DEFAULT_LABEL_WIDTH;
            label.Top = Height;
            label.Anchor = AnchorStyles.Top | AnchorStyles.Left;
            Controls.Add(label);

            m_yTextBox = new MetroTextBox();
            MetroSkinManager.ApplyMetroStyle(m_yTextBox);
            m_yTextBox.Width = Width - label.Width;
            m_yTextBox.Top = Height;
            m_yTextBox.Left = label.Width;
            m_yTextBox.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
            m_yTextBox.TextChanged += new EventHandler(m_yTextBox_TextChanged);
            Controls.Add(m_yTextBox);

            Height += m_yTextBox.Height;
        }

        private void m_xTextBox_TextChanged(object sender, EventArgs e)
        {
            if (!ValidateValue())
                return;

            var ov = Value;
            T v = ov[0];
            if (Utils.TryParse<T>(m_xTextBox.Text, out v, v, Settings.DefaultNumberStyle, Settings.DefaultFormatProvider))
                Value = new List<T>(new T[] { v, ov[1] });
        }

        private void m_yTextBox_TextChanged(object sender, EventArgs e)
        {
            if (!ValidateValue())
                return;

            var ov = Value;
            T v = ov[1];
            if (Utils.TryParse<T>(m_yTextBox.Text, out v, v, Settings.DefaultNumberStyle, Settings.DefaultFormatProvider))
                Value = new List<T>(new T[] { ov[0], v });
        }
    }
}
