using System;
using MetroFramework.Forms;
using MetroFramework.Controls;
using ZasuvkaPtakopyskaExtender;
using System.Drawing;
using System.Windows.Forms;
using MetroFramework;

namespace ZasuvkaPtakopyska
{
    public class OpenFileWithDialog : MetroForm
    {
        #region Private Static Data.

        private static readonly int DEFAULT_SEPARATOR = 8;
        private static readonly int DEFAULT_BUTTON_HEIGHT = 24;
        private static readonly int DEFAULT_SIZE = 320;

        #endregion



        #region Private Data.

        private string[] m_options;
        private int m_selectedOption = -1;
        private int m_contentHeight = 0;

        #endregion



        #region Public Properties.

        public int ResultOptionIndex
        {
            get { return m_selectedOption; }
            set { m_selectedOption = m_options == null || value < 0 || value > m_options.Length - 1 ? -1 : value; }
        }
        public string ResultOption
        {
            get
            {
                return m_options == null || m_selectedOption < 0 || m_selectedOption > m_options.Length - 1 ?
                        null :
                        m_options[m_selectedOption];
            }
            set
            {
                m_selectedOption = -1;
                if (m_options != null && value != null)
                {
                    for (int i = 0; i < m_options.Length; ++i)
                        if (m_options[i] == value)
                            m_selectedOption = i;
                }
            }
        }
        public int OptionsCount { get { return m_options == null ? 0 : m_options.Length; } }

        #endregion



        #region Construction and Destruction.

        public OpenFileWithDialog(string options = "", string defaultButton = "Default Application")
            : this(String.IsNullOrEmpty(options) ? null : options.Split('|'), defaultButton)
        {
        }

        public OpenFileWithDialog(string[] options, string defaultButton = "Default Application")
        {
            MetroSkinManager.ApplyMetroStyle(this);
            Text = "Open File With";
            TextAlign = MetroFormTextAlign.Center;
            Size = new Size(DEFAULT_SIZE, 0);
            ShowInTaskbar = false;
            ControlBox = false;
            Resizable = false;
            DialogResult = DialogResult.None;
            m_options = options;

            MetroPanel panel = new MetroPanel();
            MetroSkinManager.ApplyMetroStyle(panel);
            panel.Size = new Size(DEFAULT_SIZE, 0);
            panel.Dock = DockStyle.Fill;
            Controls.Add(panel);

            int index = 0;
            m_contentHeight = 0;
            MetroTileIcon tile;
            if (m_options != null)
            {
                foreach (string opt in m_options)
                {
                    tile = new MetroTileIcon();
                    MetroSkinManager.ApplyMetroStyle(tile);
                    tile.Tag = index;
                    tile.Width = panel.Width;
                    tile.Height = DEFAULT_BUTTON_HEIGHT;
                    tile.Top = m_contentHeight;
                    tile.Text = opt;
                    tile.TextAlign = ContentAlignment.MiddleCenter;
                    tile.TileTextFontWeight = MetroTileTextWeight.Bold;
                    tile.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
                    tile.Click += new EventHandler(tile_Click);
                    panel.Controls.Add(tile);
                    m_contentHeight = tile.Bottom + DEFAULT_SEPARATOR;
                    ++index;
                }
            }

            tile = new MetroTileIcon();
            MetroSkinManager.ApplyMetroStyle(tile);
            tile.Tag = -1;
            tile.Width = panel.Width;
            tile.Height = DEFAULT_BUTTON_HEIGHT;
            tile.Top = m_contentHeight;
            tile.Text = defaultButton;
            tile.TextAlign = ContentAlignment.MiddleCenter;
            tile.TileTextFontWeight = MetroTileTextWeight.Bold;
            tile.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
            tile.Click += new EventHandler(tile_Click);
            panel.Controls.Add(tile);
            m_contentHeight = tile.Bottom + DEFAULT_SEPARATOR;

            tile = new MetroTileIcon();
            MetroSkinManager.ApplyMetroStyle(tile);
            tile.Width = panel.Width;
            tile.Height = DEFAULT_BUTTON_HEIGHT;
            tile.Top = DEFAULT_SEPARATOR + DEFAULT_SEPARATOR + m_contentHeight;
            tile.Text = "CANCEL";
            tile.TextAlign = ContentAlignment.MiddleCenter;
            tile.TileTextFontWeight = MetroTileTextWeight.Bold;
            tile.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
            tile.Click += new EventHandler(tile_Click);
            panel.Controls.Add(tile);
            m_contentHeight = tile.Bottom;

            panel.Height = m_contentHeight;
            Height = m_contentHeight + Padding.Vertical;
        }

        private void tile_Click(object sender, EventArgs e)
        {
            MetroTileIcon tile = sender as MetroTileIcon;
            if (tile == null)
                return;

            if (tile.Tag is int)
                ResultOptionIndex = (int)tile.Tag;
            if (tile.Tag != null)
                DialogResult = DialogResult.OK;
            Close();
        }

        #endregion
    }
}
