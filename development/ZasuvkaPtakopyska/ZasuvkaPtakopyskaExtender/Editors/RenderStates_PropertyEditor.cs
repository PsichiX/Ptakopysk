using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace ZasuvkaPtakopyskaExtender.Editors
{
    [PtakopyskPropertyEditor("sf::RenderStates")]
    [PtakopyskPropertyEditor("RenderStates", TypePriority = 1)]
    public class RenderStates_PropertyEditor : PropertyEditor<Dictionary<string, object>>, IAssetsModelRequired
    {
        private static readonly string[] BLEND_MODE_VALUES = new string[] { "BlendAlpha", "BlendAdd", "BlendMultiply", "BlendNone" };

        private EnumPropertyEditor m_blendModeEditor;
        private ShaderAsset_PropertyEditor m_assetEditor;

        public SceneModel.Assets AssetsModel { get { return m_assetEditor.AssetsModel; } set { m_assetEditor.AssetsModel = value; } }

        public RenderStates_PropertyEditor(object propertyOwner, string propertyName)
            : base(propertyOwner, propertyName)
        {
            InitializeComponent();
        }

        public RenderStates_PropertyEditor(Dictionary<string, object> properties, string propertyName)
            : base(properties, propertyName)
        {
            InitializeComponent();
        }

        public override void UpdateEditorValue()
        {
        }

        private void InitializeComponent()
        {
            m_blendModeEditor = new EnumPropertyEditor(Value, "blendMode", BLEND_MODE_VALUES);
            m_blendModeEditor.Width = Width - 20;
            m_blendModeEditor.Top = Height;
            m_blendModeEditor.Left = 10;
            m_blendModeEditor.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
            Controls.Add(m_blendModeEditor);

            Height += m_blendModeEditor.Height + 10;

            m_assetEditor = new ShaderAsset_PropertyEditor(Value, "shader");
            m_assetEditor.Width = Width - 20;
            m_assetEditor.Top = Height;
            m_assetEditor.Left = 10;
            m_assetEditor.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
            Controls.Add(m_assetEditor);

            Height += m_assetEditor.Height;
        }
    }
}
