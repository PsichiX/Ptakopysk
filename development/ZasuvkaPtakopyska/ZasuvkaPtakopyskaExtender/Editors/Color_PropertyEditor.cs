using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using MetroFramework.Controls;
using System.Windows.Forms;
using System.Drawing;

namespace ZasuvkaPtakopyskaExtender.Editors
{
    [PtakopyskPropertyEditor("sf::Color")]
    [PtakopyskPropertyEditor("Color", TypePriority = 1)]
    public class Color_PropertyEditor : PropertyEditor<List<int>>
    {
        public class RGBPickerControl : Control
        {
            public delegate void ColorChangedDelegate(Color c);
            public event ColorChangedDelegate ColorChanged;

            public Color Color { get { return Color.FromArgb(255, BackColor); } set { BackColor = Color.FromArgb(255, value); if (ColorChanged != null) ColorChanged(Color); } }

            protected override void OnPaint(PaintEventArgs e)
            {
                Graphics g = e.Graphics;
                Brush brush = new SolidBrush(Color);
                Pen pen = new Pen(Color.Black);
                Rectangle rect = ClientRectangle;
                g.FillRectangle(brush, rect);
                g.DrawRectangle(pen, rect);
                pen = new Pen(Color.White);
                rect.X += 1;
                rect.Y += 1;
                rect.Width -= 2;
                rect.Height -= 2;
                g.DrawRectangle(pen, rect);
            }
        }

        private static readonly int DEFAULT_HEIGHT = 32;

        private RGBPickerControl m_rgbPicker;
        private MetroTextBox m_aTextBox;
        private MetroTrackBar m_aTrack;

        public Color Color
        {
            get { return Color.FromArgb(m_aTrack.Value, m_rgbPicker.Color); }
            set { m_rgbPicker.Color = value; m_aTrack.Value = value.A; }
        }

        public Color_PropertyEditor(Dictionary<string, string> properties, string propertyName)
            : base(properties, propertyName)
        {
            InitializeComponent();
        }

        public override void UpdateEditorValue()
        {
            if (!ValidateValue())
                return;

            var ov = Value;
            Color = Color.FromArgb(ov[3], ov[0], ov[1], ov[2]);
        }

        private bool ValidateValue()
        {
            var ov = Value;
            if (ov == null)
            {
                Value = DefaultValue;
                ov = Value;
            }
            return ov != null && ov.Count >= 4;
        }

        private void InitializeComponent()
        {
            m_rgbPicker = new RGBPickerControl();
            m_rgbPicker.Top = Height;
            m_rgbPicker.Width = DEFAULT_HEIGHT;
            m_rgbPicker.Height = DEFAULT_HEIGHT;
            m_rgbPicker.Anchor = AnchorStyles.Top | AnchorStyles.Left;
            m_rgbPicker.Click += new EventHandler(m_rgbPicker_Click);
            m_rgbPicker.ColorChanged += new RGBPickerControl.ColorChangedDelegate(m_rgbPicker_ColorChanged);
            Controls.Add(m_rgbPicker);

            m_aTextBox = new MetroTextBox();
            MetroSkinManager.ApplyMetroStyle(m_aTextBox);
            m_aTextBox.Top = Height + (m_rgbPicker.Height / 2) - (m_aTextBox.Height / 2);
            m_aTextBox.Left = m_rgbPicker.Right + 5;
            m_aTextBox.Width = 40;
            m_aTextBox.Anchor = AnchorStyles.Top | AnchorStyles.Left;
            m_aTextBox.TextChanged += new EventHandler(m_aTextBox_TextChanged);
            Controls.Add(m_aTextBox);

            m_aTrack = new MetroTrackBar();
            MetroSkinManager.ApplyMetroStyle(m_aTrack);
            m_aTrack.Top = Height;
            m_aTrack.Left = m_aTextBox.Right;
            m_aTrack.Width = Width - m_aTextBox.Right - 10;
            m_aTrack.Height = m_rgbPicker.Height;
            m_aTrack.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
            m_aTrack.Maximum = 255;
            m_aTrack.ValueChanged += new EventHandler(m_aTrack_ValueChanged);
            Controls.Add(m_aTrack);

            Height += m_rgbPicker.Height;
        }

        private void m_aTextBox_TextChanged(object sender, EventArgs e)
        {
            int v = m_aTrack.Value;
            if (!int.TryParse(m_aTextBox.Text, out v))
                return;

            v = Math.Max(m_aTrack.Minimum, Math.Min(m_aTrack.Maximum, v));

            m_aTrack.ValueChanged -= new EventHandler(m_aTrack_ValueChanged);
            m_aTrack.Value = v;
            m_aTrack.ValueChanged += new EventHandler(m_aTrack_ValueChanged);

            if (!ValidateValue())
                return;

            var ov = Value;
            Value = new List<int>(new int[] { ov[0], ov[1], ov[2], m_aTrack.Value });
        }

        private void m_aTrack_ValueChanged(object sender, EventArgs e)
        {
            m_aTextBox.TextChanged -= new EventHandler(m_aTextBox_TextChanged);
            m_aTextBox.Text = m_aTrack.Value.ToString();
            m_aTextBox.TextChanged += new EventHandler(m_aTextBox_TextChanged);

            if (!ValidateValue())
                return;

            var ov = Value;
            Value = new List<int>(new int[] { ov[0], ov[1], ov[2], m_aTrack.Value });
        }

        private void m_rgbPicker_ColorChanged(Color c)
        {
            if (!ValidateValue())
                return;

            Value = new List<int>(new int[] { Color.R, Color.G, Color.B, Value[3] });
        }

        private void m_rgbPicker_Click(object sender, EventArgs e)
        {
            ColorDialog dialog = new ColorDialog();
            dialog.Color = m_rgbPicker.Color;
            dialog.AllowFullOpen = true;
            DialogResult result = dialog.ShowDialog();
            if (result == DialogResult.OK)
                m_rgbPicker.Color = dialog.Color;
        }
    }
}
