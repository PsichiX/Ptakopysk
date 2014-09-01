using System.Collections.Generic;
using System.Windows.Forms;
using System.Globalization;
using Newtonsoft.Json.Linq;

namespace ZasuvkaPtakopyskaExtender.Editors
{
    [PtakopyskPropertyEditor("b2Filter")]
    public class Filter_PropertyEditor : PropertyEditor<object>, IEditorJsonValueChangedCallback
    {
        private ParsablePropertyEditor<ushort> m_categoryEditor;
        private ParsablePropertyEditor<ushort> m_maskEditor;
        private ParsablePropertyEditor<short> m_groupEditor;
        private Dictionary<string, string> m_subProperties = new Dictionary<string, string>();

        public Filter_PropertyEditor(Dictionary<string, string> properties, string propertyName)
            : base(properties, propertyName)
        {
            InitializeComponent();
        }

        public override void UpdateEditorValue()
        {
            try
            {
                JObject data = Newtonsoft.Json.JsonConvert.DeserializeObject<JObject>(JsonValue);
                if (data != null)
                {
                    JToken v;
                    if (data.TryGetValue("category", out v))
                    {
                        m_subProperties["category"] = v.ToString(Newtonsoft.Json.Formatting.None);
                        m_categoryEditor.UpdateEditorValue();
                    }
                    if (data.TryGetValue("mask", out v))
                    {
                        m_subProperties["mask"] = v.ToString(Newtonsoft.Json.Formatting.None);
                        m_maskEditor.UpdateEditorValue();
                    }
                    if (data.TryGetValue("group", out v))
                    {
                        m_subProperties["group"] = v.ToString(Newtonsoft.Json.Formatting.None);
                        m_groupEditor.UpdateEditorValue();
                    }
                }
            }
            catch { }
        }

        public void OnEditorValueChanged(IEditorJsonValue editor, string property, string jsonValue)
        {
            m_subProperties[property] = jsonValue;
            if (m_subProperties.ContainsKey("category") && m_subProperties.ContainsKey("mask") && m_subProperties.ContainsKey("group"))
                JsonValue = "{ \"category\": " + m_subProperties["category"] + ", \"mask\": " + m_subProperties["mask"] + ", \"group\": " + m_subProperties["group"] + " }";
        }

        private void InitializeComponent()
        {
            IsProxyEditor = true;

            m_subProperties["category"] = "null";
            m_subProperties["mask"] = "null";
            m_subProperties["group"] = "null";
            
            m_categoryEditor = new ParsablePropertyEditor<ushort>(m_subProperties, "category");
            m_categoryEditor.Text += " (hex)";
            m_categoryEditor.StringFormat = "X4";
            m_categoryEditor.NumberStyle = NumberStyles.HexNumber;
            m_categoryEditor.Width = Width - 20;
            m_categoryEditor.Top = Height;
            m_categoryEditor.Left = 10;
            m_categoryEditor.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
            Controls.Add(m_categoryEditor);
            m_categoryEditor.EditorJsonValueChangedCallback = this;

            Height += m_categoryEditor.Height + 10;

            m_maskEditor = new ParsablePropertyEditor<ushort>(m_subProperties, "mask");
            m_maskEditor.Text += " (hex)";
            m_maskEditor.StringFormat = "X4";
            m_maskEditor.NumberStyle = NumberStyles.HexNumber;
            m_maskEditor.Width = Width - 20;
            m_maskEditor.Top = Height;
            m_maskEditor.Left = 10;
            m_maskEditor.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
            Controls.Add(m_maskEditor);
            m_maskEditor.EditorJsonValueChangedCallback = this;

            Height += m_maskEditor.Height + 10;

            m_groupEditor = new ParsablePropertyEditor<short>(m_subProperties, "group");
            m_groupEditor.Width = Width - 20;
            m_groupEditor.Top = Height;
            m_groupEditor.Left = 10;
            m_groupEditor.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
            Controls.Add(m_groupEditor);
            m_groupEditor.EditorJsonValueChangedCallback = this;

            Height += m_groupEditor.Height;
        }
    }
}
