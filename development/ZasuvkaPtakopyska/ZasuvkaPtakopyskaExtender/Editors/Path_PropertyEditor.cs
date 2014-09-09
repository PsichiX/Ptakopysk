using System;
using MetroFramework.Controls;
using System.Windows.Forms;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using MetroFramework;

namespace ZasuvkaPtakopyskaExtender.Editors
{
    [PtakopyskPropertyEditor("@Path")]
    public class Path_PropertyEditor : PropertyEditor<string>
    {
        private static readonly int DEFAULT_SELECT_TILE_WIDTH = 48;

        public string RootPath { get; set; }
        public bool IsDirectoryPath { get; set; }
        public string FileFilter { get; set; }

        private MetroTextBox m_textBox;
        private MetroTile m_button;

        public Path_PropertyEditor(Dictionary<string, string> properties, string propertyName)
            : base(properties, propertyName)
        {
            RootPath = "";
            IsDirectoryPath = false;
            FileFilter = "All files (*.*)|*.*";
            InitializeComponent();
        }

        public override void UpdateEditorValue()
        {
            m_textBox.Text = Value;
        }

        private void InitializeComponent()
        {
            m_textBox = new MetroTextBox();
            MetroSkinManager.ApplyMetroStyle(m_textBox);
            m_textBox.Width = Width - DEFAULT_SELECT_TILE_WIDTH;
            m_textBox.Top = Height;
            m_textBox.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
            m_textBox.TextChanged += new EventHandler(m_textBox_TextChanged);
            Controls.Add(m_textBox);

            m_button = new MetroTile();
            MetroSkinManager.ApplyMetroStyle(m_button);
            m_button.Tag = m_textBox;
            m_button.Text = "...";
            m_button.TextAlign = ContentAlignment.MiddleCenter;
            m_button.TileTextFontSize = MetroTileTextSize.Tall;
            m_button.TileTextFontWeight = MetroTileTextWeight.Bold;
            m_button.Width = DEFAULT_SELECT_TILE_WIDTH;
            m_button.Height = m_textBox.Height;
            m_button.Left = m_textBox.Right;
            m_button.Top = Height;
            m_button.Anchor = AnchorStyles.Top | AnchorStyles.Right;
            m_button.Click += new EventHandler(m_button_Click_dir);
            Controls.Add(m_button);

            Height += m_textBox.Height;
        }

        private void m_textBox_TextChanged(object sender, EventArgs e)
        {
            Value = m_textBox.Text;
        }

        private void m_button_Click_dir(object sender, EventArgs e)
        {
            MetroTile button = sender as MetroTile;
            if (button == null || button.Tag == null)
                return;

            MetroTextBox textBox = button.Tag as MetroTextBox;
            if (textBox == null)
                return;

            DialogResult result = DialogResult.None;
            string path = textBox.Text;
            if (!Path.IsPathRooted(path))
                path = RootPath + path;
            path = Path.GetFullPath(path.Replace('/', '\\'));
            if (IsDirectoryPath)
            {
                FolderBrowserDialog dialog = new FolderBrowserDialog();
                dialog.ShowNewFolderButton = true;
                if (!String.IsNullOrEmpty(textBox.Text))
                    dialog.SelectedPath = path;
                result = dialog.ShowDialog();
                path = dialog.SelectedPath;
            }
            else
            {
                OpenFileDialog dialog = new OpenFileDialog();
                if (!String.IsNullOrEmpty(textBox.Text))
                    dialog.FileName = path;
                dialog.Filter = FileFilter;
                result = dialog.ShowDialog();
                path = dialog.FileName;
            }
            if (result == DialogResult.OK)
            {
                result = MetroMessageBox.Show(FindForm(), "Keep relative path?", "Question", MessageBoxButtons.YesNo, MessageBoxIcon.Question);
                if (result == DialogResult.Yes)
                    path = Utils.GetRelativePath(path, RootPath);
                textBox.Text = path;
            }
        }
    }
}
