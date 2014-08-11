using System.Collections.Generic;
using System.Windows.Forms;

namespace ZasuvkaPtakopyskaExtender.Editors
{
    [PtakopyskPropertyEditor("sf::RenderStates")]
    [PtakopyskPropertyEditor("RenderStates", TypePriority = 1)]
    public class RenderStates_PropertyEditor : PropertyEditor<object>, IEditorJsonValueChangedCallback
    {
        private static readonly string[] BLEND_MODE_VALUES = new string[] { "BlendAlpha", "BlendAdd", "BlendMultiply", "BlendNone" };

        private EnumPropertyEditor m_blendModeEditor;
        private ShaderAsset_PropertyEditor m_shaderAssetEditor;
        private Dictionary<string, string> m_subProperties = new Dictionary<string, string>();

        public RenderStates_PropertyEditor(Dictionary<string, string> properties, string propertyName)
            : base(properties, propertyName)
        {
            InitializeComponent();
        }

        public override void UpdateEditorValue()
        {
            m_subProperties["blendMode"] = m_blendModeEditor.JsonValue;
            m_subProperties["shader"] = m_shaderAssetEditor.JsonDefaultValue;
        }

        public void OnEditorValueChanged(IEditorJsonValue editor, string property, string jsonValue)
        {
            m_subProperties[property] = jsonValue;
            if (m_subProperties.ContainsKey("blendMode") && m_subProperties.ContainsKey("shader"))
                JsonValue = "{ \"blendMode\": " + m_subProperties["blendMode"] + ", \"shader\": " + m_subProperties["shader"] + " }";
        }

        private void InitializeComponent()
        {
            IsProxyEditor = true;

            m_subProperties["blendMode"] = "null";
            m_subProperties["shader"] = "null";
            
            m_blendModeEditor = new EnumPropertyEditor(m_subProperties, "blendMode", BLEND_MODE_VALUES);
            m_blendModeEditor.Width = Width - 20;
            m_blendModeEditor.Top = Height;
            m_blendModeEditor.Left = 10;
            m_blendModeEditor.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
            Controls.Add(m_blendModeEditor);
            m_blendModeEditor.EditorJsonValueChangedCallback = this;

            Height += m_blendModeEditor.Height + 10;

            m_shaderAssetEditor = new ShaderAsset_PropertyEditor(m_subProperties, "shader");
            m_shaderAssetEditor.Width = Width - 20;
            m_shaderAssetEditor.Top = Height;
            m_shaderAssetEditor.Left = 10;
            m_shaderAssetEditor.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
            Controls.Add(m_shaderAssetEditor);
            m_shaderAssetEditor.EditorJsonValueChangedCallback = this;

            Height += m_shaderAssetEditor.Height;
        }
    }
}
