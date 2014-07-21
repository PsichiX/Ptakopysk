using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using MetroFramework.Controls;
using ZasuvkaPtakopyskaExtender;
using System.IO;
using System.Windows.Forms;
using System.Drawing;
using MetroFramework;

namespace ZasuvkaPtakopyska
{
    public class ProjectFilesControl : MetroPanel
    {
        #region Private Static Data.

        private static readonly Size DEFAULT_TILE_SIZE = new Size(192, 64);

        #endregion



        #region Private Data.

        private string m_rootPath;
        private string m_viewPath;
        private FlowLayoutPanel m_flowPanel;
        private Image m_backImage;
        private Image m_dirImage;
        private Image m_fileImage;
        private Image m_fileCodeImage;
        private Image m_fileImageImage;
        private Image m_fileMusicImage;
        private Image m_fileTextImage;
        private Image m_fileDomImage;

        #endregion



        #region Public Properties.

        public string RootPath { get { return m_rootPath; } set { m_rootPath = value; RebuildList(); } }
        public string ViewPath { get { return m_viewPath; } set { m_viewPath = value; RebuildList(); } }

        #endregion



        #region Construction and Destruction.

        public ProjectFilesControl()
        {
            MetroSkinManager.ApplyMetroStyle(this);
            AutoScroll = true;
            Resize += new EventHandler(ProjectFilesControl_Resize);

            m_flowPanel = new FlowLayoutPanel();
            m_flowPanel.FlowDirection = FlowDirection.LeftToRight;
            Controls.Add(m_flowPanel);

            m_backImage = Bitmap.FromFile("resources/icons/appbar.arrow.left.png");
            m_dirImage = Bitmap.FromFile("resources/icons/appbar.folder.png");
            m_fileImage = Bitmap.FromFile("resources/icons/appbar.page.png");
            m_fileCodeImage = Bitmap.FromFile("resources/icons/appbar.page.code.png");
            m_fileImageImage = Bitmap.FromFile("resources/icons/appbar.page.image.png");
            m_fileMusicImage = Bitmap.FromFile("resources/icons/appbar.page.music.png");
            m_fileTextImage = Bitmap.FromFile("resources/icons/appbar.page.text.png");
            m_fileDomImage = Bitmap.FromFile("resources/icons/appbar.page.xml.png");
        }

        private void ProjectFilesControl_Resize(object sender, EventArgs e)
        {
            m_flowPanel.Width = Width;
            m_flowPanel.Height = Height;
        }

        #endregion



        #region Public Functionality.

        public void RebuildList()
        {
            m_flowPanel.Controls.Clear();

            if (m_viewPath == null || m_rootPath == null)
                m_viewPath = m_rootPath;

            if (m_viewPath == null || !Directory.Exists(m_viewPath))
                return;

            string root = Path.GetFullPath(m_rootPath);
            string path = Path.GetFullPath(m_viewPath);
            
            MetroTileIcon tile;
            bool isRoot = path.Length == root.Length;
            DirectoryInfo dir = new DirectoryInfo(path);
            if (!isRoot)
            {
                tile = new MetroTileIcon();
                MetroSkinManager.ApplyMetroStyle(tile);
                tile.Tag = dir.FullName + @"\..";
                tile.Size = DEFAULT_TILE_SIZE;
                tile.Image = m_backImage;
                tile.Click += new EventHandler(tile_Click);
                m_flowPanel.Controls.Add(tile);
            }
            foreach (DirectoryInfo info in dir.GetDirectories())
            {
                tile = new MetroTileIcon();
                MetroSkinManager.ApplyMetroStyle(tile);
                tile.Tag = info.FullName;
                tile.Size = DEFAULT_TILE_SIZE;
                tile.Text = info.Name;
                tile.TextAlign = ContentAlignment.BottomRight;
                tile.TileTextFontWeight = MetroTileTextWeight.Bold;
                tile.Image = m_dirImage;
                tile.ImageAlign = ContentAlignment.TopLeft;
                tile.ImageOffset = new Point(-14, -14);
                tile.Click += new EventHandler(tile_Click);
                m_flowPanel.Controls.Add(tile);
            }
            string ext;
            foreach (FileInfo info in dir.GetFiles())
            {
                ext = Path.GetExtension(info.Name);
                tile = new MetroTileIcon();
                MetroSkinManager.ApplyMetroStyle(tile);
                tile.Tag = info.FullName;
                tile.Size = DEFAULT_TILE_SIZE;
                tile.Text = (ext.Length < 1 ? "" : ext.Substring(1).ToUpperInvariant()) + "\n" + Path.GetFileNameWithoutExtension(info.Name);
                tile.TextAlign = ContentAlignment.BottomRight;
                tile.TileTextFontWeight = MetroTileTextWeight.Bold;
                if (ext == ".h" || ext == ".cpp")
                    tile.Image = m_fileCodeImage;
                else if (ext == ".png" || ext == ".jpg" || ext == ".jpeg" || ext == ".bmp")
                    tile.Image = m_fileImageImage;
                else if (ext == ".ogg" || ext == ".mp3" || ext == ".wav")
                    tile.Image = m_fileMusicImage;
                else if (ext == ".txt" || ext == ".log")
                    tile.Image = m_fileTextImage;
                else if (ext == ".json" || ext == ".xml")
                    tile.Image = m_fileDomImage;
                else
                    tile.Image = m_fileImage;
                tile.ImageAlign = ContentAlignment.TopLeft;
                tile.ImageOffset = new Point(-14, -14);
                tile.Click += new EventHandler(tile_Click);
                m_flowPanel.Controls.Add(tile);
            }
        }

        public void OpenFile(string path)
        {
            MainForm mainForm = FindForm() as MainForm;
            if (mainForm != null)
                mainForm.OpenEditFile(path);
        }

        #endregion



        #region Private Events Handlers.

        private void tile_Click(object sender, EventArgs e)
        {
            MetroTileIcon tile = sender as MetroTileIcon;
            if (tile == null)
                return;

            string path = tile.Tag as string;
            if (Directory.Exists(path))
                ViewPath = path;
            else if (File.Exists(path))
                OpenFile(path);
        }

        #endregion
    }
}
