using System;
using MetroFramework.Controls;
using ZasuvkaPtakopyskaExtender;
using ZasuvkaPtakopyskaExtender.Editors;
using System.Windows.Forms;
using PtakopyskMetaGenerator;
using System.Collections.Generic;
using MetroFramework;

namespace ZasuvkaPtakopyska
{
    public class GameObjectPropertiesEditor : MetroPanel
    {
        #region Private Static Data.

        private static readonly int DEFAULT_SEPARATOR = 8;

        #endregion



        #region Private Data.

        private SceneModel.GameObject m_model;

        #endregion



        #region Construction and Destruction.

        public GameObjectPropertiesEditor(SceneModel.GameObject model)
        {
            if (model == null)
                throw new ArgumentNullException("Ptakopysk Component model cannot be null!");

            m_model = model;
            if (m_model.properties == null)
                m_model.properties = new SceneModel.GameObject.Properties();

            MetroSkinManager.ApplyMetroStyle(this);
            AutoScroll = true;

            int y = DEFAULT_SEPARATOR;
            y = InitializePropertiesSection(y);
            y = InitializeComponentsSection(y);
        }

        #endregion



        #region Private Functionality.

        private int InitializePropertiesSection(int y)
        {
            StringPropertyEditor idEditor = new StringPropertyEditor(m_model.properties, "Id");
            idEditor.Top = y;
            idEditor.Width = Width;
            idEditor.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
            Controls.Add(idEditor);
            y = idEditor.Bottom + DEFAULT_SEPARATOR;

            BoolPropertyEditor activeEditor = new BoolPropertyEditor(m_model.properties, "Active");
            activeEditor.Top = y;
            activeEditor.Width = Width;
            activeEditor.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
            Controls.Add(activeEditor);
            y = activeEditor.Bottom + DEFAULT_SEPARATOR;

            IntPropertyEditor orderEditor = new IntPropertyEditor(m_model.properties, "Order");
            orderEditor.Top = y;
            orderEditor.Width = Width;
            orderEditor.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
            Controls.Add(orderEditor);
            y = orderEditor.Bottom + DEFAULT_SEPARATOR;

            JsonStringPropertyEditor metaDataEditor = new JsonStringPropertyEditor(m_model.properties, "MetaData");
            metaDataEditor.Top = y;
            metaDataEditor.Width = Width;
            metaDataEditor.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
            Controls.Add(metaDataEditor);
            y = metaDataEditor.Bottom + DEFAULT_SEPARATOR;

            return y;
        }

        private int InitializeComponentsSection(int y)
        {
            if (m_model == null || m_model.components == null || m_model.components.Count == 0)
                return y;

            foreach (SceneModel.GameObject.Component comp in m_model.components)
                y = InitializeComponentFragment(comp, y);

            return y;
        }

        private int InitializeComponentFragment(SceneModel.GameObject.Component comp, int y)
        {
            if (comp == null)
                return y;

            MetaComponent meta = MetaComponentsManager.Instance.FindMetaComponent(comp.type);
            if (meta == null)
                return y;

            MetroButton btn = new MetroButton();
            MetroSkinManager.ApplyMetroStyle(btn);
            btn.Text = comp.type;
            btn.FontWeight = MetroButtonWeight.Bold;
            btn.Top = y;
            btn.Width = Width;
            btn.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
            Controls.Add(btn);
            y = btn.Bottom + DEFAULT_SEPARATOR;

            List<MetaProperty> props = MetaComponentsManager.Instance.GetFlattenPropertiesOf(meta);
            if (props == null || props.Count == 0)
                return y;

            foreach (MetaProperty p in props)
            {
                Type t = PropertyEditorsManager.Instance.FindPropertyEditorByValueType(p.ValueType);
            }

            return y;
        }

        #endregion
    }
}
