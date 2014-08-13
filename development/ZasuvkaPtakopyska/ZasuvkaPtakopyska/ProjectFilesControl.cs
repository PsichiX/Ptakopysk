﻿using System;
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
    public class ProjectFilesControl : MetroPanel, MetroSidePanel.IMetroSidePanelScrollableContent
    {
        #region Private Static Data.

        private static readonly int DEFAULT_SEPARATOR = 4;
        private static readonly Size DEFAULT_TILE_SIZE = new Size(74, 74);

        #endregion



        #region Private Data.

        private string m_rootPath;
        private string m_viewPath;
        private MetroPanel m_content;
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

        public MetroSidePanel SidePanel { get; set; }
        public Rectangle ScrollableContentRectangle
        {
            get
            {
                Rectangle rect;
                m_content.CalculateContentsRectangle(out rect);
                return rect;
            }
        }
        public string RootPath { get { return m_rootPath; } set { m_rootPath = value; RebuildList(); } }
        public string ViewPath { get { return m_viewPath; } set { m_viewPath = value; RebuildList(); } }
        public int VerticalScrollValue { get { return m_content.VerticalScroll.Value; } set { m_content.VerticalScroll.Value = value; } }
        public int VerticalScrollMaximum { get { return m_content.VerticalScroll.Maximum; } set { m_content.VerticalScroll.Maximum = value; } }
        public int VerticalScrollLargeChange { get { return m_content.VerticalScroll.LargeChange; } set { m_content.VerticalScroll.LargeChange = value; } }
        public int HorizontalScrollValue { get { return m_content.HorizontalScroll.Value; } set { m_content.HorizontalScroll.Value = value; } }
        public int HorizontalScrollMaximum { get { return m_content.HorizontalScroll.Maximum; } set { m_content.HorizontalScroll.Maximum = value; } }
        public int HorizontalScrollLargeChange { get { return m_content.HorizontalScroll.LargeChange; } set { m_content.HorizontalScroll.LargeChange = value; } }

        #endregion



        #region Construction and Destruction.

        public ProjectFilesControl()
        {
            MetroSkinManager.ApplyMetroStyle(this);

            m_content = new MetroPanel();
            m_content.Dock = DockStyle.Fill;
            Controls.Add(m_content);

            m_backImage = Bitmap.FromFile("resources/icons/appbar.arrow.left.png");
            m_dirImage = Bitmap.FromFile("resources/icons/appbar.folder.png");
            m_fileImage = Bitmap.FromFile("resources/icons/appbar.page.png");
            m_fileCodeImage = Bitmap.FromFile("resources/icons/appbar.page.code.png");
            m_fileImageImage = Bitmap.FromFile("resources/icons/appbar.page.image.png");
            m_fileMusicImage = Bitmap.FromFile("resources/icons/appbar.page.music.png");
            m_fileTextImage = Bitmap.FromFile("resources/icons/appbar.page.text.png");
            m_fileDomImage = Bitmap.FromFile("resources/icons/appbar.page.xml.png");

            RebuildList();
        }

        #endregion



        #region Public Functionality.

        public void RebuildList()
        {
            m_content.Controls.Clear();

            if (m_viewPath == null || m_rootPath == null)
                m_viewPath = m_rootPath;

            if (m_viewPath == null || !Directory.Exists(m_viewPath))
                return;

            string root = Path.GetFullPath(m_rootPath);
            string path = Path.GetFullPath(m_viewPath);
            bool upDownRow = false;
            int x = DEFAULT_SEPARATOR;

            MetroTileIcon tile;
            bool isRoot = path.Length == root.Length;
            DirectoryInfo dir = new DirectoryInfo(path);
            if (!isRoot)
            {
                tile = new MetroTileIcon();
                MetroSkinManager.ApplyMetroStyle(tile);
                tile.Tag = dir.FullName + @"\..";
                tile.Top = DEFAULT_SEPARATOR;
                tile.Left = x;
                tile.Size = DEFAULT_TILE_SIZE;
                tile.Image = m_backImage;
                tile.IsImageScaled = true;
                tile.ImageScale = new PointF(0.85f, 0.85f);
                tile.Click += new EventHandler(tile_Click);
                m_content.Controls.Add(tile);
                upDownRow = !upDownRow;
            }
            foreach (DirectoryInfo info in dir.GetDirectories())
            {
                tile = new MetroTileIcon();
                MetroSkinManager.ApplyMetroStyle(tile);
                tile.Tag = info.FullName;
                tile.Top = upDownRow ? (DEFAULT_SEPARATOR + DEFAULT_TILE_SIZE.Height + DEFAULT_SEPARATOR) : DEFAULT_SEPARATOR;
                tile.Left = x;
                tile.Size = DEFAULT_TILE_SIZE;
                tile.Text = info.Name;
                tile.TextAlign = ContentAlignment.BottomRight;
                tile.TileTextFontSize = MetroTileTextSize.Small;
                tile.TileTextFontWeight = MetroTileTextWeight.Light;
                tile.Image = m_dirImage;
                tile.IsImageScaled = true;
                tile.ImageScale = new PointF(0.85f, 0.85f);
                tile.ImageAlign = ContentAlignment.TopLeft;
                tile.ImageOffset = new Point(-10, -10);
                tile.Click += new EventHandler(tile_Click);
                m_content.Controls.Add(tile);
                upDownRow = !upDownRow;
                if (!upDownRow)
                    x += DEFAULT_TILE_SIZE.Width + DEFAULT_SEPARATOR;
            }
            string ext;
            foreach (FileInfo info in dir.GetFiles())
            {
                ext = Path.GetExtension(info.Name);
                tile = new MetroTileIcon();
                MetroSkinManager.ApplyMetroStyle(tile);
                tile.Tag = info.FullName;
                tile.Top = upDownRow ? (DEFAULT_SEPARATOR + DEFAULT_TILE_SIZE.Height + DEFAULT_SEPARATOR) : DEFAULT_SEPARATOR;
                tile.Left = x;
                tile.Size = DEFAULT_TILE_SIZE;
                tile.Text = ext + "\n" + Path.GetFileNameWithoutExtension(info.Name);
                tile.TextAlign = ContentAlignment.BottomRight;
                tile.TileTextFontSize = MetroTileTextSize.Small;
                tile.TileTextFontWeight = MetroTileTextWeight.Light;
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
                tile.IsImageScaled = true;
                tile.ImageScale = new PointF(0.85f, 0.85f);
                tile.ImageAlign = ContentAlignment.TopLeft;
                tile.ImageOffset = new Point(-10, -10);
                tile.Click += new EventHandler(tile_Click);
                m_content.Controls.Add(tile);
                upDownRow = !upDownRow;
                if (!upDownRow)
                    x += DEFAULT_TILE_SIZE.Width + DEFAULT_SEPARATOR;
            }

            if (SidePanel != null)
                SidePanel.UpdateScrollbars();
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



        #region Protected Functionality.

        protected override void OnResize(EventArgs eventargs)
        {
            base.OnResize(eventargs);
            if (SidePanel != null)
                SidePanel.UpdateScrollbars();
        }

        #endregion
    }
}
