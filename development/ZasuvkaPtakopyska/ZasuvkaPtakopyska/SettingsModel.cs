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

        #endregion



        #region Construction and Destruction.

        public SettingsModel()
        {
            SdkPath = "..";
            CodeBlocksIdePath = Utils.GetCodeBlocksInstallationPath();
            BashBinPath = "";
            UiStyle = MetroFramework.MetroColorStyle.Blue;
            UiTheme = MetroFramework.MetroThemeStyle.Dark;
        }

        #endregion
    }
}
