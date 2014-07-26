using System;
using MetroFramework.Controls;
using System.Windows.Forms;
using System.Collections.Generic;

namespace ZasuvkaPtakopyskaExtender.Editors
{
    [PtakopyskPropertyEditor("sf::Vector2f")]
    [PtakopyskPropertyEditor("Vector2f", TypePriority = 1)]
    public class Vector2fPropertyEditor : PropertyEditor<List<float>>
    {
        private MetroTextBox m_xTextBox;
        private MetroTextBox m_yTextBox;

        public Vector2fPropertyEditor(object propertyOwner, string propertyName)
            : base(propertyOwner, propertyName, new List<float>())
        {
            ValidateValue();
            InitializeComponent();
        }

        public Vector2fPropertyEditor(Dictionary<string, object> properties, string propertyName)
            : base(properties, propertyName, new List<float>())
        {
            ValidateValue();
            InitializeComponent();
        }

        public override void UpdateEditorValue()
        {
            ValidateValue();

            m_xTextBox.Text = Value[0].ToString();
            m_yTextBox.Text = Value[1].ToString();
        }

        private void ValidateValue()
        {
            if (Value == null)
                Value = new List<float>(DefaultValue);
            if (Value.Count < 1)
                Value.Add(0.0f);
            if (Value.Count < 2)
                Value.Add(0.0f);
        }

        private void InitializeComponent()
        {
            MetroLabel label;

            label = new MetroLabel();
            MetroSkinManager.ApplyMetroStyle(label);
            label.Text = "X:";
            label.Width = 30;
            label.Top = Height;
            label.Anchor = AnchorStyles.Top | AnchorStyles.Left;
            Controls.Add(label);

            m_xTextBox = new MetroTextBox();
            MetroSkinManager.ApplyMetroStyle(m_xTextBox);
            m_xTextBox.Text = Value[0].ToString();
            m_xTextBox.Width = Width;
            m_xTextBox.Top = Height;
            m_xTextBox.Left = label.Width;
            m_xTextBox.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
            m_xTextBox.TextChanged += new EventHandler(m_xTextBox_TextChanged);
            Controls.Add(m_xTextBox);

            Height += m_xTextBox.Height;

            label = new MetroLabel();
            MetroSkinManager.ApplyMetroStyle(label);
            label.Text = "Y:";
            label.Width = 30;
            label.Top = Height;
            label.Anchor = AnchorStyles.Top | AnchorStyles.Left;
            Controls.Add(label);

            m_yTextBox = new MetroTextBox();
            MetroSkinManager.ApplyMetroStyle(m_yTextBox);
            m_yTextBox.Text = Value[1].ToString();
            m_yTextBox.Width = Width;
            m_yTextBox.Top = Height;
            m_yTextBox.Left = label.Width;
            m_yTextBox.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
            m_yTextBox.TextChanged += new EventHandler(m_yTextBox_TextChanged);
            Controls.Add(m_yTextBox);

            Height += m_yTextBox.Height;
        }

        private void m_xTextBox_TextChanged(object sender, EventArgs e)
        {
            float v = Value[0];
            if (float.TryParse(m_xTextBox.Text, out v))
                Value[0] = v;
        }

        private void m_yTextBox_TextChanged(object sender, EventArgs e)
        {
            float v = Value[1];
            if (float.TryParse(m_yTextBox.Text, out v))
                Value[1] = v;
        }
    }
}
