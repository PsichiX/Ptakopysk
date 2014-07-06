using System;
using System.Drawing;
using System.IO;
using System.Windows.Forms;
using MetroFramework.Controls;
using Newtonsoft.Json;
using MetroFramework.Components;
using MetroFramework;
using System.Collections.Generic;

namespace ZasuvkaPtakopyska
{
    public partial class SettingsPageControl : MetroPanel
    {
        #region Private Static Data.

        private static readonly string SETTINGS_FILE_PATH = "Settings.json";
        private static readonly int DEFAULT_SEPARATOR = 24;
        private static readonly int DEFAULT_SELECT_TILE_WIDTH = 48;
        
        #endregion

        
        
        #region Private Data.

        private SettingsModel m_settingsModel;
        private MetroComboBox m_styleComboBox;
        private MetroComboBox m_themeComboBox;

        #endregion



        #region Public Properties.

        public SettingsModel SettingsModel
        {
            get { return m_settingsModel; }
        }

        #endregion



        #region Construction & Destruction.

        public SettingsPageControl()
        {
            LoadSettingsModel();

            InitializeContents();
        }

        #endregion



        #region Private Functionality.

        private void InitializeContents()
        {
            MetroSkinManager.ApplyMetroStyle(this);
            Disposed += new EventHandler(SettingsPageControl_Disposed);
            AutoScroll = true;

            MetroLabel label;
            MetroTextBox textBox;
            MetroTileIcon button;
            
            label = new MetroLabel();
            MetroSkinManager.ApplyMetroStyle(label);
            label.Size = new Size();
            label.AutoSize = true;
            label.Text = "SDK Location:";
            label.Location = new Point(DEFAULT_SEPARATOR, DEFAULT_SEPARATOR);
            Controls.Add(label);

            textBox = new MetroTextBox();
            MetroSkinManager.ApplyMetroStyle(textBox);
            textBox.Location = new Point(DEFAULT_SEPARATOR, label.Bottom);
            textBox.Width = Width - DEFAULT_SELECT_TILE_WIDTH - DEFAULT_SEPARATOR - DEFAULT_SEPARATOR;
            textBox.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
            textBox.Text = m_settingsModel.SdkPath;
            textBox.TextChanged += new EventHandler(textBox_TextChanged_sdk);
            Controls.Add(textBox);

            button = new MetroTileIcon();
            MetroSkinManager.ApplyMetroStyle(button);
            button.Tag = textBox;
            button.Location = new Point(textBox.Right, textBox.Top);
            button.Width = DEFAULT_SELECT_TILE_WIDTH;
            button.Height = textBox.Height;
            button.Anchor = AnchorStyles.Top | AnchorStyles.Right;
            button.Image = Bitmap.FromFile("resources/icons/appbar.select.mini.png");
            button.ImageAlign = ContentAlignment.MiddleCenter;
            button.Click += new EventHandler(button_Click_dir);
            Controls.Add(button);

            label = new MetroLabel();
            MetroSkinManager.ApplyMetroStyle(label);
            label.Size = new Size();
            label.AutoSize = true;
            label.Text = "Code::Blocks IDE Location:";
            label.Location = new Point(DEFAULT_SEPARATOR, textBox.Bottom + DEFAULT_SEPARATOR);
            Controls.Add(label);

            textBox = new MetroTextBox();
            MetroSkinManager.ApplyMetroStyle(textBox);
            textBox.Location = new Point(DEFAULT_SEPARATOR, label.Bottom);
            textBox.Width = Width - DEFAULT_SELECT_TILE_WIDTH - DEFAULT_SEPARATOR - DEFAULT_SEPARATOR;
            textBox.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
            textBox.Text = m_settingsModel.CodeBlocksIdePath;
            textBox.TextChanged += new EventHandler(textBox_TextChanged_cb);
            Controls.Add(textBox);

            button = new MetroTileIcon();
            MetroSkinManager.ApplyMetroStyle(button);
            button.Tag = textBox;
            button.Location = new Point(textBox.Right, textBox.Top);
            button.Width = DEFAULT_SELECT_TILE_WIDTH;
            button.Height = textBox.Height;
            button.Anchor = AnchorStyles.Top | AnchorStyles.Right;
            button.Image = Bitmap.FromFile("resources/icons/appbar.select.mini.png");
            button.ImageAlign = ContentAlignment.MiddleCenter;
            button.Click += new EventHandler(button_Click_dir);
            Controls.Add(button);

            label = new MetroLabel();
            MetroSkinManager.ApplyMetroStyle(label);
            label.Size = new Size();
            label.AutoSize = true;
            label.Text = "Bash executable Location:";
            label.Location = new Point(DEFAULT_SEPARATOR, textBox.Bottom + DEFAULT_SEPARATOR);
            Controls.Add(label);

            textBox = new MetroTextBox();
            MetroSkinManager.ApplyMetroStyle(textBox);
            textBox.Location = new Point(DEFAULT_SEPARATOR, label.Bottom);
            textBox.Width = Width - DEFAULT_SELECT_TILE_WIDTH - DEFAULT_SEPARATOR - DEFAULT_SEPARATOR;
            textBox.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
            textBox.Text = m_settingsModel.BashBinPath;
            textBox.TextChanged += new EventHandler(textBox_TextChanged_sh);
            Controls.Add(textBox);

            button = new MetroTileIcon();
            MetroSkinManager.ApplyMetroStyle(button);
            button.Tag = textBox;
            button.Location = new Point(textBox.Right, textBox.Top);
            button.Width = DEFAULT_SELECT_TILE_WIDTH;
            button.Height = textBox.Height;
            button.Anchor = AnchorStyles.Top | AnchorStyles.Right;
            button.Image = Bitmap.FromFile("resources/icons/appbar.select.mini.png");
            button.ImageAlign = ContentAlignment.MiddleCenter;
            button.Click += new EventHandler(button_Click_file);
            Controls.Add(button);

            label = new MetroLabel();
            MetroSkinManager.ApplyMetroStyle(label);
            label.Size = new Size();
            label.AutoSize = true;
            label.Text = "Application Style:";
            label.Location = new Point(DEFAULT_SEPARATOR, textBox.Bottom + DEFAULT_SEPARATOR);
            Controls.Add(label);

            m_styleComboBox = new MetroComboBox();
            MetroSkinManager.ApplyMetroStyle(m_styleComboBox);
            m_styleComboBox.Location = new Point(DEFAULT_SEPARATOR, label.Bottom);
            Controls.Add(m_styleComboBox);

            label = new MetroLabel();
            MetroSkinManager.ApplyMetroStyle(label);
            label.Size = new Size();
            label.AutoSize = true;
            label.Text = "Application Theme:";
            label.Location = new Point(DEFAULT_SEPARATOR, m_styleComboBox.Bottom + DEFAULT_SEPARATOR);
            Controls.Add(label);

            m_themeComboBox = new MetroComboBox();
            MetroSkinManager.ApplyMetroStyle(m_themeComboBox);
            m_themeComboBox.Location = new Point(DEFAULT_SEPARATOR, label.Bottom);
            Controls.Add(m_themeComboBox);
        }

        #endregion



        #region Public Functionality.

        public void RefreshContent()
        {
            m_styleComboBox.SelectedValueChanged -= new EventHandler(comboBox_SelectedValueChanged);
            List<string> items = new List<string>();
            foreach (string item in Enum.GetNames(typeof(MetroColorStyle)))
                items.Add(item);
            m_styleComboBox.DataSource = items;
            string style = m_settingsModel.UiStyle.ToString();
            m_styleComboBox.SelectedItem = style;
            m_styleComboBox.SelectedValueChanged += new EventHandler(comboBox_SelectedValueChanged);
            
            items = new List<string>();
            foreach (string item in Enum.GetNames(typeof(MetroThemeStyle)))
                items.Add(item);
            m_themeComboBox.SelectedValueChanged -= new EventHandler(comboBox_SelectedValueChanged);
            m_themeComboBox.DataSource = items;
            string theme = m_settingsModel.UiTheme.ToString();
            m_themeComboBox.SelectedItem = theme;
            m_themeComboBox.SelectedValueChanged += new EventHandler(comboBox_SelectedValueChanged);
        }

        public void LoadSettingsModel()
        {
            string data = File.Exists(SETTINGS_FILE_PATH) ? File.ReadAllText(SETTINGS_FILE_PATH) : "{}";
            m_settingsModel = JsonConvert.DeserializeObject<SettingsModel>(data);
            if (m_settingsModel == null)
                m_settingsModel = new SettingsModel();
            MetroSkinManager.Style = m_settingsModel.UiStyle;
            MetroSkinManager.Theme = m_settingsModel.UiTheme;
        }

        public void SaveSettingsModel()
        {
            string data = JsonConvert.SerializeObject(m_settingsModel, Formatting.Indented);
            File.WriteAllText(SETTINGS_FILE_PATH, data);
        }
        
        #endregion



        #region Private Events Handlers.

        private void SettingsPageControl_Disposed(object sender, EventArgs e)
        {
            SaveSettingsModel();
        }

        private void textBox_TextChanged_sdk(object sender, EventArgs e)
        {
            m_settingsModel.SdkPath = (sender as MetroTextBox).Text;
        }

        private void textBox_TextChanged_cb(object sender, EventArgs e)
        {
            m_settingsModel.CodeBlocksIdePath = (sender as MetroTextBox).Text;
        }

        private void textBox_TextChanged_sh(object sender, EventArgs e)
        {
            m_settingsModel.BashBinPath = (sender as MetroTextBox).Text;
        }

        private void button_Click_dir(object sender, EventArgs e)
        {
            MetroTileIcon button = sender as MetroTileIcon;
            if (button == null || button.Tag == null)
                return;
            
            MetroTextBox textBox = button.Tag as MetroTextBox;
            if (textBox == null)
                return;
            
            FolderBrowserDialog dialog = new FolderBrowserDialog();
            dialog.ShowNewFolderButton = true;
            dialog.SelectedPath = Path.GetFullPath(textBox.Text);
            DialogResult result = dialog.ShowDialog();
            if (result == DialogResult.OK)
            {
                string path = dialog.SelectedPath;
                result = MetroMessageBox.Show(FindForm(), "Keep relative path?", "Question", MessageBoxButtons.YesNo, MessageBoxIcon.Question);
                if (result == DialogResult.Yes)
                    path = Utils.GetRelativePath(path, Application.ExecutablePath);
                textBox.Text = path;
            }
        }

        private void button_Click_file(object sender, EventArgs e)
        {
            MetroTileIcon button = sender as MetroTileIcon;
            if (button == null || button.Tag == null)
                return;
            
            MetroTextBox textBox = button.Tag as MetroTextBox;
            if (textBox == null)
                return;
            
            OpenFileDialog dialog = new OpenFileDialog();
            dialog.CheckPathExists = true;
            dialog.RestoreDirectory = true;
            dialog.FileName = Path.GetFullPath(textBox.Text);
            dialog.Filter = "Executable file (*.exe)|*.exe";
            DialogResult result = dialog.ShowDialog();
            if (result == DialogResult.OK)
            {
                string path = dialog.FileName;
                result = MetroMessageBox.Show(FindForm(), "Keep relative path?", "Question", MessageBoxButtons.YesNo, MessageBoxIcon.Question);
                if (result == DialogResult.Yes)
                    path = Utils.GetRelativePath(path, Application.ExecutablePath);
                textBox.Text = path;
            }
        }

        private void comboBox_SelectedValueChanged(object sender, EventArgs e)
        {
            MetroComboBox comboBox = sender as MetroComboBox;
            if (comboBox == null)
                return;

            if (comboBox == m_styleComboBox)
            {
                MetroColorStyle style = m_settingsModel.UiStyle;
                Enum.TryParse<MetroColorStyle>(comboBox.SelectedItem.ToString(), out style);
                m_settingsModel.UiStyle = style;
                MetroSkinManager.Style = style;
            }
            else if (comboBox == m_themeComboBox)
            {
                MetroThemeStyle theme = m_settingsModel.UiTheme;
                Enum.TryParse<MetroThemeStyle>(comboBox.SelectedItem.ToString(), out theme);
                m_settingsModel.UiTheme = theme;
                MetroSkinManager.Theme = theme;
            }
        }
        
        #endregion
    }
}
