using System.IO;
using System;
using System.Windows.Forms;
namespace ZasuvkaPtakopyska
{
    public class SettingsModel
    {
        #region Public Properties.

        public string SdkPath { get; set; }
        public string CodeBlocksIdePath { get; set; }
        public string BashBinPath { get; set; }
        public MetroFramework.MetroColorStyle UiStyle { get; set; }
        public MetroFramework.MetroThemeStyle UiTheme { get; set; }
        public FormWindowState WindowState { get; set; }

        #endregion



        #region Construction and Destruction.

        public SettingsModel()
        {
            SdkPath = "..";
            CodeBlocksIdePath = Utils.GetCodeBlocksInstallationPath();
            BashBinPath = "";
            UiStyle = MetroFramework.MetroColorStyle.Blue;
            UiTheme = MetroFramework.MetroThemeStyle.Dark;
            WindowState = FormWindowState.Normal;
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

        public bool ValidateBashBinPath()
        {
            return  !String.IsNullOrEmpty(BashBinPath)
                    && File.Exists(BashBinPath);
        }

        #endregion
    }
}
