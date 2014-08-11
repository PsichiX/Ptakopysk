using System;
using MetroFramework.Controls;
using System.Windows.Forms;
using System.Collections.Generic;

namespace ZasuvkaPtakopyskaExtender.Editors
{
    [PtakopyskPropertyEditor("b2Vec2")]
    [PtakopyskPropertyEditor("sf::Vector2f")]
    [PtakopyskPropertyEditor("Vector2f", TypePriority = 1)]
    public class FloatVector2_PropertyEditor : PropertyEditor<List<float>>
    {
        private static readonly int DEFAULT_LABEL_WIDTH = 30;

        private MetroTextBox m_xTextBox;
        private MetroTextBox m_yTextBox;

        public FloatVector2_PropertyEditor(Dictionary<string, string> properties, string propertyName)
            : base(properties, propertyName)
        {
            InitializeComponent();
        }

        public override void UpdateEditorValue()
        {
            if (!ValidateValue())
                return;

            m_xTextBox.Text = Value[0].ToString();
            m_yTextBox.Text = Value[1].ToString();
        }

        private bool ValidateValue()
        {
            var ov = Value;
            if (ov == null)
            {
                Value = new List<float>(DefaultValue);
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
            float v = ov[0];
            if (float.TryParse(m_xTextBox.Text, out v))
                Value = new List<float>(new float[] { v, ov[1] });
        }

        private void m_yTextBox_TextChanged(object sender, EventArgs e)
        {
            if (!ValidateValue())
                return;

            var ov = Value;
            float v = Value[1];
            if (float.TryParse(m_yTextBox.Text, out v))
                Value = new List<float>(new float[] { ov[0], v });
        }
    }
}
