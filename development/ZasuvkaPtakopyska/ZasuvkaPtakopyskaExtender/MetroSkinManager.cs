using System;
using MetroFramework;
using MetroFramework.Interfaces;
using MetroFramework.Components;
using System.Windows.Forms;
using MetroFramework.Forms;

namespace ZasuvkaPtakopyskaExtender
{
    public class MetroSkinManager
    {
        #region Private Static Data.

        private static MetroStyleManager MANAGER = new MetroStyleManager();
        private static MetroStyleExtender EXTENDER;

        #endregion



        #region Public Static Properties.

        public static MetroColorStyle Style
        {
            get { GetExtender(); return MANAGER.Style; }
            set { GetExtender(); MANAGER.Style = value; }
        }
        public static MetroThemeStyle Theme
        {
            get { GetExtender(); return MANAGER.Theme; }
            set { GetExtender(); MANAGER.Theme = value; }
        }

        #endregion



        #region Private Static Functionality

        private static MetroStyleExtender GetExtender()
        {
            if (EXTENDER == null)
            {
                EXTENDER = new MetroStyleExtender();
                EXTENDER.StyleManager = MANAGER;
                EXTENDER.Style = MetroColorStyle.Default;
                EXTENDER.Theme = MetroThemeStyle.Default;
            }
            return EXTENDER;
        }
        
        #endregion.



        #region Public Static Functionality.

        public static void ApplyMetroStyle(IMetroControl control)
        {
            if (control == null)
                return;

            control.StyleManager = MANAGER;
            Control legacyControl = control as Control;
            if (legacyControl != null)
                legacyControl.Disposed += new EventHandler(legacyControl_Disposed);
        }

        public static void ApplyMetroStyle(IMetroForm form)
        {
            if (form == null)
                return;

            form.StyleManager = MANAGER;
            Control legacyForm = form as Form;
            if (legacyForm != null)
                legacyForm.Disposed += new EventHandler(legacyForm_Disposed);
        }

        public static void ExtendMetroStyle(Control control)
        {
            if (control == null)
                return;

            GetExtender();
            EXTENDER.SetApplyMetroTheme(control, true);
        }

        public static void SetManagerOwner(MetroForm form)
        {
            MANAGER.Owner = form;
        }

        public static void RefreshStyles()
        {
            MANAGER.Update();
        }

        #endregion



        #region Private Static Events Handlers.

        private static void legacyControl_Disposed(object sender, EventArgs e)
        {
            IMetroControl control = sender as IMetroControl;
            if (control != null)
                control.StyleManager = null;

            Control legacyControl = sender as Control;
            if (legacyControl != null)
                legacyControl.Disposed -= new EventHandler(legacyControl_Disposed);
        }

        private static void legacyForm_Disposed(object sender, EventArgs e)
        {
            IMetroForm form = sender as IMetroForm;
            if (form != null)
                form.StyleManager = null;

            Form legacyForm = sender as Form;
            if (legacyForm != null)
                legacyForm.Disposed -= new EventHandler(legacyForm_Disposed);
        }
        
        #endregion
    }
}
