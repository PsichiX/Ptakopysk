using System.Collections.Generic;
using System.Windows.Forms;

namespace ZasuvkaPtakopyskaExtender.Editors
{
    [PtakopyskPropertyEditor("RenderMaterial")]
    public class Material_PropertyEditor : PropertyEditor<object>, IEditorJsonValueChangedCallback
    {
        public class TextureAssetsCollection_PropertyEditor : CollectionPropertyEditor<string>
        {
            public TextureAssetsCollection_PropertyEditor(Dictionary<string, string> properties, string propertyName)
                : base(
                properties,
                propertyName,
                CollectionPropertyEditorUtils.CollectionType.JsonObject,
                (pd, pn) => new TextureAsset_PropertyEditor(pd, pn)
                )
            {
            }
        }

        public class FloatCollection_PropertyEditor : CollectionPropertyEditor<float>
        {
            public FloatCollection_PropertyEditor(Dictionary<string, string> properties, string propertyName)
                : base(
                properties,
                propertyName,
                CollectionPropertyEditorUtils.CollectionType.JsonArray,
                (pd, pn) => new Float_PropertyEditor(pd, pn)
                )
            {
            }
        }

        public class PropertiesCollection_PropertyEditor : CollectionPropertyEditor<List<float>>
        {
            public PropertiesCollection_PropertyEditor(Dictionary<string, string> properties, string propertyName)
                : base(
                properties,
                propertyName,
                CollectionPropertyEditorUtils.CollectionType.JsonObject,
                (pd, pn) => new FloatCollection_PropertyEditor(pd, pn)
                )
            {
            }
        }

        private Dictionary<string, string> m_subProperties = new Dictionary<string, string>();
        private PropertiesCollection_PropertyEditor m_propertiesEditor;
        private TextureAssetsCollection_PropertyEditor m_texturesEditor;

        public Material_PropertyEditor(Dictionary<string, string> properties, string propertyName)
            : base(properties, propertyName)
        {
            InitializeComponent();
        }

        public override void UpdateEditorValue()
        {
        }

        public void OnEditorValueChanged(IEditorJsonValue editor, string property, string jsonValue)
        {
            m_subProperties[property] = jsonValue;
            if (m_subProperties.ContainsKey("properties") && m_subProperties.ContainsKey("textures"))
                JsonValue = "{ \"properties\": " + m_subProperties["properties"] + ", \"textures\": " + m_subProperties["textures"] + " }";
        }

        private void InitializeComponent()
        {
            IsProxyEditor = true;

            m_subProperties["properties"] = "null";
            m_subProperties["textures"] = "null";

            m_propertiesEditor = new PropertiesCollection_PropertyEditor(m_subProperties, "properties");
            m_propertiesEditor.Width = Width - 20;
            m_propertiesEditor.Top = Height;
            m_propertiesEditor.Left = 10;
            m_propertiesEditor.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
            Controls.Add(m_propertiesEditor);
            m_propertiesEditor.EditorJsonValueChangedCallback = this;

            Height += m_propertiesEditor.Height;

            m_texturesEditor = new TextureAssetsCollection_PropertyEditor(m_subProperties, "textures");
            m_texturesEditor.Width = Width - 20;
            m_texturesEditor.Top = Height;
            m_texturesEditor.Left = 10;
            m_texturesEditor.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
            Controls.Add(m_texturesEditor);
            m_texturesEditor.EditorJsonValueChangedCallback = this;

            Height += m_texturesEditor.Height;
        }
    }
}
