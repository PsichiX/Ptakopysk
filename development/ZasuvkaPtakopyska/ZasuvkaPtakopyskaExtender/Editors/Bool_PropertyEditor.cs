using System;
using MetroFramework.Controls;
using System.Collections.Generic;

namespace ZasuvkaPtakopyskaExtender.Editors
{
    [PtakopyskPropertyEditor("bool")]
    public class Bool_PropertyEditor : PropertyEditor<bool>
    {
        private MetroToggle m_toggle;

        public Bool_PropertyEditor(Dictionary<string, string> properties, string propertyName)
            : base(properties, propertyName)
        {
            InitializeComponent();
        }

        public override void UpdateEditorValue()
        {
            m_toggle.Checked = Value;
        }

        private void InitializeComponent()
        {
            m_toggle = new MetroToggle();
            MetroSkinManager.ApplyMetroStyle(m_toggle);
            m_toggle.Top = Height;
            m_toggle.CheckedChanged += new EventHandler(m_toggle_CheckedChanged);
            Controls.Add(m_toggle);

            Height += m_toggle.Height;
        }

        private void m_toggle_CheckedChanged(object sender, EventArgs e)
        {
            Value = m_toggle.Checked;
        }
    }
}
