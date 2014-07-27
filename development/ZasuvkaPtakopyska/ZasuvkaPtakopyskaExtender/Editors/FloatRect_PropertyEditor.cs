using System;
using MetroFramework.Controls;
using System.Windows.Forms;
using System.Collections.Generic;

namespace ZasuvkaPtakopyskaExtender.Editors
{
    [PtakopyskPropertyEditor("sf::FloatRect")]
    [PtakopyskPropertyEditor("FloatRect", TypePriority = 1)]
    public class FloatRect_PropertyEditor : PropertyEditor<List<float>>
    {
        private static readonly int DEFAULT_LABEL_WIDTH = 60;
        
        private MetroTextBox m_leftTextBox;
        private MetroTextBox m_topTextBox;
        private MetroTextBox m_widthTextBox;
        private MetroTextBox m_heightTextBox;

        public FloatRect_PropertyEditor(object propertyOwner, string propertyName)
            : base(propertyOwner, propertyName)
        {
            InitializeComponent();
        }

        public FloatRect_PropertyEditor(Dictionary<string, object> properties, string propertyName)
            : base(properties, propertyName)
        {
            InitializeComponent();
        }

        public override void UpdateEditorValue()
        {
            if (!ValidateValue())
                return;

            m_leftTextBox.Text = Value[0].ToString();
            m_topTextBox.Text = Value[1].ToString();
            m_widthTextBox.Text = Value[2].ToString();
            m_heightTextBox.Text = Value[3].ToString();
        }

        private bool ValidateValue()
        {
            if (Value == null && DefaultValue == null)
                Value = new List<float>(DefaultValue);
            return Value != null && Value.Count >= 4;
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
            m_leftTextBox.Width = Width;
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
            m_topTextBox.Width = Width;
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
            m_widthTextBox.Width = Width;
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
            m_heightTextBox.Width = Width;
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
            
            float v = Value[0];
            if (float.TryParse(m_leftTextBox.Text, out v))
                Value[0] = v;
        }

        private void m_topTextBox_TextChanged(object sender, EventArgs e)
        {
            if (!ValidateValue())
                return;

            float v = Value[1];
            if (float.TryParse(m_topTextBox.Text, out v))
                Value[1] = v;
        }

        private void m_widthTextBox_TextChanged(object sender, EventArgs e)
        {
            if (!ValidateValue())
                return;

            float v = Value[2];
            if (float.TryParse(m_topTextBox.Text, out v))
                Value[2] = v;
        }

        private void m_heightTextBox_TextChanged(object sender, EventArgs e)
        {
            if (!ValidateValue())
                return;

            float v = Value[3];
            if (float.TryParse(m_topTextBox.Text, out v))
                Value[3] = v;
        }
    }
}
