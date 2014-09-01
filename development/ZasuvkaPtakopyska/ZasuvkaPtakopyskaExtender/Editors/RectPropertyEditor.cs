using System;
using MetroFramework.Controls;
using System.Windows.Forms;
using System.Collections.Generic;
using System.Globalization;
using System.Reflection;

namespace ZasuvkaPtakopyskaExtender.Editors
{
    [PtakopyskPropertyEditor("@Rect")]
    [PtakopyskPropertyEditor("char[4]", AliasValueType = "@Rect<System.SByte>")]
    [PtakopyskPropertyEditor("byte[4]", AliasValueType = "@Rect<System.Byte>")]
    [PtakopyskPropertyEditor("short[4]", AliasValueType = "@Rect<System.Int16>")]
    [PtakopyskPropertyEditor("word[4]", AliasValueType = "@Rect<System.UInt16>")]
    [PtakopyskPropertyEditor("int[4]", AliasValueType = "@Rect<System.Int32>")]
    [PtakopyskPropertyEditor("dword[4]", AliasValueType = "@Rect<System.UInt32>")]
    [PtakopyskPropertyEditor("long[4]", AliasValueType = "@Rect<System.Int64>")]
    [PtakopyskPropertyEditor("qword[4]", AliasValueType = "@Rect<System.UInt64>")]
    [PtakopyskPropertyEditor("float[4]", AliasValueType = "@Rect<System.Single>")]
    [PtakopyskPropertyEditor("double[4]", AliasValueType = "@Rect<System.Double>")]
    [PtakopyskPropertyEditor("sf::IntRect", AliasValueType = "@Rect<System.Int32>")]
    [PtakopyskPropertyEditor("sf::FloatRect", AliasValueType = "@Rect<System.Single>")]
    [PtakopyskPropertyEditor("sf::Rect<int>", AliasValueType = "@Rect<System.Int32>")]
    [PtakopyskPropertyEditor("sf::Rect<float>", AliasValueType = "@Rect<System.Single>")]
    [PtakopyskPropertyEditor("IntRect", AliasValueType = "@Rect<System.Int32>", TypePriority = 1)]
    [PtakopyskPropertyEditor("FloatRect", AliasValueType = "@Rect<System.Single>", TypePriority = 1)]
    [PtakopyskPropertyEditor("Rect<int>", AliasValueType = "@Rect<System.Int32>", TypePriority = 1)]
    [PtakopyskPropertyEditor("Rect<float>", AliasValueType = "@Rect<System.Single>", TypePriority = 1)]
    public class RectPropertyEditor<T> : PropertyEditor<List<T>> where T : IFormattable
    {
        private static readonly int DEFAULT_LABEL_WIDTH = 60;

        public string StringFormat { get; set; }
        public NumberStyles NumberStyle { get; set; }
        public IFormatProvider FormatProvider { get; set; }

        private MetroTextBox m_leftTextBox;
        private MetroTextBox m_topTextBox;
        private MetroTextBox m_widthTextBox;
        private MetroTextBox m_heightTextBox;

        public RectPropertyEditor(Dictionary<string, string> properties, string propertyName)
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
            m_leftTextBox.Text = ov[0].ToString(StringFormat, FormatProvider);
            m_topTextBox.Text = ov[1].ToString(StringFormat, FormatProvider);
            m_widthTextBox.Text = ov[2].ToString(StringFormat, FormatProvider);
            m_heightTextBox.Text = ov[3].ToString(StringFormat, FormatProvider);
        }

        private bool ValidateValue()
        {
            var ov = Value;
            var dv = DefaultValue;
            if (ov == null && dv != null)
            {
                Value = dv == null || dv.Count < 4 ? new List<T>(new T[] { default(T), default(T), default(T), default(T) }) : new List<T>(dv);
                ov = Value;
            }
            return ov != null && ov.Count >= 4;
        }

        private void InitializeComponent()
        {
            MetroLabel label;

            label = new MetroLabel();
            MetroSkinManager.ApplyMetroStyle(label);
            label.Text = "Left:";
            label.Width = DEFAULT_LABEL_WIDTH;
            label.Top = Height;
            label.Anchor = AnchorStyles.Top | AnchorStyles.Left;
            Controls.Add(label);

            m_leftTextBox = new MetroTextBox();
            MetroSkinManager.ApplyMetroStyle(m_leftTextBox);
            m_leftTextBox.Width = Width - label.Width;
            m_leftTextBox.Top = Height;
            m_leftTextBox.Left = label.Width;
            m_leftTextBox.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
            m_leftTextBox.TextChanged += new EventHandler(m_leftTextBox_TextChanged);
            Controls.Add(m_leftTextBox);

            Height += m_leftTextBox.Height;

            label = new MetroLabel();
            MetroSkinManager.ApplyMetroStyle(label);
            label.Text = "Top:";
            label.Width = DEFAULT_LABEL_WIDTH;
            label.Top = Height;
            label.Anchor = AnchorStyles.Top | AnchorStyles.Left;
            Controls.Add(label);

            m_topTextBox = new MetroTextBox();
            MetroSkinManager.ApplyMetroStyle(m_topTextBox);
            m_topTextBox.Width = Width - label.Width;
            m_topTextBox.Top = Height;
            m_topTextBox.Left = label.Width;
            m_topTextBox.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
            m_topTextBox.TextChanged += new EventHandler(m_topTextBox_TextChanged);
            Controls.Add(m_topTextBox);

            Height += m_topTextBox.Height;

            label = new MetroLabel();
            MetroSkinManager.ApplyMetroStyle(label);
            label.Text = "Width:";
            label.Width = DEFAULT_LABEL_WIDTH;
            label.Top = Height;
            label.Anchor = AnchorStyles.Top | AnchorStyles.Left;
            Controls.Add(label);

            m_widthTextBox = new MetroTextBox();
            MetroSkinManager.ApplyMetroStyle(m_widthTextBox);
            m_widthTextBox.Width = Width - label.Width;
            m_widthTextBox.Top = Height;
            m_widthTextBox.Left = label.Width;
            m_widthTextBox.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
            m_widthTextBox.TextChanged += new EventHandler(m_widthTextBox_TextChanged);
            Controls.Add(m_widthTextBox);

            Height += m_widthTextBox.Height;

            label = new MetroLabel();
            MetroSkinManager.ApplyMetroStyle(label);
            label.Text = "Height:";
            label.Width = DEFAULT_LABEL_WIDTH;
            label.Top = Height;
            label.Anchor = AnchorStyles.Top | AnchorStyles.Left;
            Controls.Add(label);

            m_heightTextBox = new MetroTextBox();
            MetroSkinManager.ApplyMetroStyle(m_heightTextBox);
            m_heightTextBox.Width = Width - label.Width;
            m_heightTextBox.Top = Height;
            m_heightTextBox.Left = label.Width;
            m_heightTextBox.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
            m_heightTextBox.TextChanged += new EventHandler(m_heightTextBox_TextChanged);
            Controls.Add(m_heightTextBox);

            Height += m_heightTextBox.Height;
        }

        private void m_leftTextBox_TextChanged(object sender, EventArgs e)
        {
            if (!ValidateValue())
                return;

            var ov = Value;
            T v = ov[0];
            if (Utils.TryParse<T>(m_leftTextBox.Text, out v, v, Settings.DefaultNumberStyle, Settings.DefaultFormatProvider))
                Value = new List<T>(new T[] { v, ov[1], ov[2], ov[3] });
        }

        private void m_topTextBox_TextChanged(object sender, EventArgs e)
        {
            if (!ValidateValue())
                return;

            var ov = Value;
            T v = ov[1];
            if (Utils.TryParse<T>(m_topTextBox.Text, out v, v, Settings.DefaultNumberStyle, Settings.DefaultFormatProvider))
                Value = new List<T>(new T[] { ov[0], v, ov[2], ov[3] });
        }

        private void m_widthTextBox_TextChanged(object sender, EventArgs e)
        {
            if (!ValidateValue())
                return;

            var ov = Value;
            T v = ov[2];
            if (Utils.TryParse<T>(m_widthTextBox.Text, out v, v, Settings.DefaultNumberStyle, Settings.DefaultFormatProvider))
                Value = new List<T>(new T[] { ov[0], ov[1], v, ov[3] });
        }

        private void m_heightTextBox_TextChanged(object sender, EventArgs e)
        {
            if (!ValidateValue())
                return;

            var ov = Value;
            T v = ov[3];
            if (Utils.TryParse<T>(m_heightTextBox.Text, out v, v, Settings.DefaultNumberStyle, Settings.DefaultFormatProvider))
                Value = new List<T>(new T[] { ov[0], ov[1], ov[2], v });
        }
    }
}
