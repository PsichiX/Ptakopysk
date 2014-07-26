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
                throw new ArgumentNullException("Game Object model cannot be null!");

            m_model = model;
            if (m_model.properties == null)
                m_model.properties = new SceneModel.GameObject.Properties();

            MetroSkinManager.ApplyMetroStyle(this);
            
            int y = DEFAULT_SEPARATOR;
            y = InitializePropertiesSection(y);
            y = InitializeComponentsSection(y);
            
            AutoScroll = true;
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
            btn.Top = y + DEFAULT_SEPARATOR + DEFAULT_SEPARATOR + DEFAULT_SEPARATOR;
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
                if (t == null)
                {
                    string msg = string.Format("Property editor for type: \"{0}\" not found!", p.ValueType);
                    ErrorPropertyEditor editor = new ErrorPropertyEditor(p.Name, msg);
                    editor.UpdateEditorValue();
                    editor.Top = y;
                    editor.Width = Width;
                    editor.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
                    Controls.Add(editor);
                    y = editor.Bottom + DEFAULT_SEPARATOR;
                }
                else
                    y = InitializePropertyFragment(comp, p, t, y);
            }

            return y;
        }

        private int InitializePropertyFragment(SceneModel.GameObject.Component component, MetaProperty property, Type editorType, int y)
        {
            if (component == null || property == null || editorType == null)
                return y;

            if (component.properties == null)
                component.properties = new Dictionary<string, object>();

            try
            {
                object obj = Activator.CreateInstance(editorType, component.properties, property.Name);
                MetroUserControl editor = obj as MetroUserControl;
                IEditorJsonValue jvEditor = editor as IEditorJsonValue;
                if (editor != null && jvEditor != null)
                {
                    if (!component.properties.ContainsKey(property.Name))
                    {
                        jvEditor.JsonDefaultValue = property.DefaultValue;
                        jvEditor.JsonValue = property.DefaultValue;
                        jvEditor.UpdateEditorValue();
                    }
                    editor.Top = y;
                    editor.Width = Width;
                    editor.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
                    Controls.Add(editor);
                    y = editor.Bottom + DEFAULT_SEPARATOR;
                }
            }
            catch(Exception ex)
            {
                while (ex.InnerException != null)
                    ex = ex.InnerException;
                ErrorPropertyEditor editor = new ErrorPropertyEditor(property.Name, ex.Message);
                editor.Top = y;
                editor.Width = Width;
                editor.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
                Controls.Add(editor);
                y = editor.Bottom + DEFAULT_SEPARATOR;
            }

            return y;
        }

        #endregion
    }
}
