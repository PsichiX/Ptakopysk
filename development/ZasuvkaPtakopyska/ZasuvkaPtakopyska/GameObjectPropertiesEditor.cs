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

        private SceneModel.GameObject m_gameObjectModel;
        private SceneModel.Assets m_assetsModel;

        #endregion



        #region Construction and Destruction.

        public GameObjectPropertiesEditor(SceneModel.GameObject gameObject, SceneModel.Assets assets)
        {
            if (gameObject == null)
                throw new ArgumentNullException("Game Object model cannot be null!");

            m_gameObjectModel = gameObject;
            if (m_gameObjectModel.properties == null)
                m_gameObjectModel.properties = new SceneModel.GameObject.Properties();
            m_assetsModel = assets;

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
            String_PropertyEditor idEditor = new String_PropertyEditor(m_gameObjectModel.properties, "Id");
            idEditor.UpdateEditorValue();
            idEditor.Top = y;
            idEditor.Width = Width;
            idEditor.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
            Controls.Add(idEditor);
            y = idEditor.Bottom + DEFAULT_SEPARATOR;

            Bool_PropertyEditor activeEditor = new Bool_PropertyEditor(m_gameObjectModel.properties, "Active");
            activeEditor.UpdateEditorValue();
            activeEditor.Top = y;
            activeEditor.Width = Width;
            activeEditor.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
            Controls.Add(activeEditor);
            y = activeEditor.Bottom + DEFAULT_SEPARATOR;

            Int_PropertyEditor orderEditor = new Int_PropertyEditor(m_gameObjectModel.properties, "Order");
            orderEditor.UpdateEditorValue();
            orderEditor.Top = y;
            orderEditor.Width = Width;
            orderEditor.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
            Controls.Add(orderEditor);
            y = orderEditor.Bottom + DEFAULT_SEPARATOR;

            JsonStringPropertyEditor metaDataEditor = new JsonStringPropertyEditor(m_gameObjectModel.properties, "MetaData");
            metaDataEditor.UpdateEditorValue();
            metaDataEditor.Top = y;
            metaDataEditor.Width = Width;
            metaDataEditor.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
            Controls.Add(metaDataEditor);
            y = metaDataEditor.Bottom + DEFAULT_SEPARATOR;

            return y;
        }

        private int InitializeComponentsSection(int y)
        {
            if (m_gameObjectModel == null || m_gameObjectModel.components == null || m_gameObjectModel.components.Count == 0)
                return y;

            foreach (SceneModel.GameObject.Component comp in m_gameObjectModel.components)
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
                    if(obj is IAssetsModelRequired)
                        (obj as IAssetsModelRequired).AssetsModel = m_assetsModel;
                    jvEditor.JsonDefaultValue = property.DefaultValue;
                    if (!component.properties.ContainsKey(property.Name))
                        jvEditor.JsonValue = property.DefaultValue;
                    jvEditor.UpdateEditorValue();
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
                editor.Tag = string.Format("{0}\n{1}\n\nStack trace:\n{2}", ex.GetType().Name, ex.Message, ex.StackTrace);
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
