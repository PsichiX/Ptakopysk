using System;
using System.IO;
using System.Windows.Forms;

namespace ZasuvkaPtakopyska
{
    public class SettingsModel
    {
        #region Public Properties.

        public string SdkPath { get; set; }
        public string CodeBlocksIdePath { get; set; }
        public MetroFramework.MetroColorStyle UiStyle { get; set; }
        public MetroFramework.MetroThemeStyle UiTheme { get; set; }
        public FormWindowState WindowState { get; set; }
        public bool LeftPanelRolled { get; set; }
        public bool LeftPanelDocked { get; set; }
        public bool RightPanelRolled { get; set; }
        public bool RightPanelDocked { get; set; }
        public bool BottomPanelRolled { get; set; }
        public bool BottomPanelDocked { get; set; }

        #endregion



        #region Construction and Destruction.

        public SettingsModel()
        {
            SdkPath = "..";
            CodeBlocksIdePath = Utils.GetCodeBlocksInstallationPath();
            UiStyle = MetroFramework.MetroColorStyle.Orange;
            UiTheme = MetroFramework.MetroThemeStyle.Dark;
            WindowState = FormWindowState.Normal;
            LeftPanelRolled = true;
            LeftPanelDocked = false;
            RightPanelRolled = true;
            RightPanelDocked = false;
            BottomPanelRolled = true;
            BottomPanelDocked = false;
        }

        #endregion



        #region Public Functionality.

        public bool ValidateSdkPath()
        {
            return  !String.IsNullOrEmpty(SdkPath)
                    && Directory.Exists(SdkPath)
                    && Directory.Exists(SdkPath + @"\bin")
                    && Directory.Exists(SdkPath + @"\IDE")
                    && Directory.Exists(SdkPath + @"\include")
                    && Directory.Exists(SdkPath + @"\lib")
                    && Directory.Exists(SdkPath + @"\templates")
                    && File.Exists(SdkPath + @"\templates\make_new_project.sh")
                    && File.Exists(SdkPath + @"\templates\make_new_component.sh");
        }

        public bool ValidateCodeBlocksIdePath()
        {
            return  !String.IsNullOrEmpty(CodeBlocksIdePath)
                    && Directory.Exists(CodeBlocksIdePath)
                    && File.Exists(CodeBlocksIdePath + @"\codeblocks.exe");
        }

        #endregion
    }
}
