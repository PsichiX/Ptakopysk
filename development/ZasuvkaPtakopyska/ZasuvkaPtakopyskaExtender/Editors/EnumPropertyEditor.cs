using System;
using System.Collections.Generic;
using MetroFramework.Controls;
using System.Windows.Forms;
using Newtonsoft.Json.Linq;

namespace ZasuvkaPtakopyskaExtender.Editors
{
    [PtakopyskPropertyEditor("@Enum")]
    public class EnumPropertyEditor : PropertyEditor<string>
    {
        private MetroComboBox m_comboBox;
        private string[] m_values;

        public static List<object> GenerateAdditionalArguments(JToken data)
        {
            if (data == null)
                return null;

            JArray a = data as JArray;
            if (a == null)
                return null;

            List<string> items = new List<string>();
            foreach (var i in a)
                if (i.Type == JTokenType.String)
                    items.Add(i.ToObject<string>());

            if (items.Count > 0)
            {
                List<object> result = new List<object>();
                result.Add(items.ToArray());
                return result;
            }
            else
                return null;
        }

        public string[] ValuesSource
        {
            get { return m_values; }
            set
            {
                m_values = value;
                m_comboBox.SelectedValueChanged -= new EventHandler(m_comboBox_SelectedValueChanged);
                m_comboBox.DataSource = m_values;
                m_comboBox.SelectedValueChanged += new EventHandler(m_comboBox_SelectedValueChanged);
                UpdateEditorValue();
            }
        }

        public EnumPropertyEditor(Dictionary<string, string> properties, string propertyName)
            : this(properties, propertyName, null)
        {
        }

        public EnumPropertyEditor(Dictionary<string, string> properties, string propertyName, string[] values)
            : base(properties, propertyName)
        {
            m_values = values;
            InitializeComponent();
        }

        public override void UpdateEditorValue()
        {
            m_comboBox.SelectedItem = Value;
        }

        private void InitializeComponent()
        {
            m_comboBox = new MetroComboBox();
            MetroSkinManager.ApplyMetroStyle(m_comboBox);
            m_comboBox.BindingContext = new BindingContext();
            m_comboBox.DataSource = m_values;
            m_comboBox.Top = Height;
            m_comboBox.Width = Width;
            m_comboBox.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
            m_comboBox.SelectedValueChanged += new EventHandler(m_comboBox_SelectedValueChanged);
            Controls.Add(m_comboBox);

            Height += m_comboBox.Height + m_comboBox.Margin.Vertical;
        }

        private void m_comboBox_SelectedValueChanged(object sender, EventArgs e)
        {
            Value = m_comboBox.SelectedValue as string;
        }
    }
}
