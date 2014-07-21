using System;
using System.Drawing;
using System.Windows.Forms;
using MetroFramework.Controls;
using MetroFramework.Animation;
using MetroFramework.Components;
using ZasuvkaPtakopyskaExtender;

namespace ZasuvkaPtakopyska
{
    public partial class MetroSidePanel : MetroUserControl
    {
        #region Public Static Data.

        public static readonly int ROLLED_PART_SIZE = 24;

        #endregion



        #region Private Static Data.

        private static readonly int ROLLING_DURATION = 15;
        private static readonly TransitionType ROLLING_TRANSITION = TransitionType.EaseInExpo;

        #endregion



        #region Private Data.

        private MetroTile m_titleBar;
        private MetroTileIcon m_dockTile;
        private Image m_dockImage;
        private Image m_undockImage;
        private MetroPanel m_content;
        private DockStyle m_side = DockStyle.None;
        private bool m_rolled = false;
        private Padding m_offsetPadding;
        private bool m_docked = false;
        private bool m_dockable = true;
        private MoveAnimation m_moveAnim;
        private Control m_lastParent;

        #endregion



        #region Public Properties

        public override string Text { get { return m_titleBar.Text; } set { m_titleBar.Text = value; } }
        public MetroPanel Content { get { return m_content; } }
        public DockStyle Side { get { return m_side; } set { m_side = value; Apply(); } }
        public bool IsRolled { get { return m_rolled; } set { m_rolled = value; if (!m_docked && m_rolled) Roll(); else Unroll(); } }
        public bool AnimatedRolling { get; set; }
        public Padding OffsetPadding { get { return m_offsetPadding; } set { m_offsetPadding = value; Apply(); } }
        public bool IsDocked
        {
            get { return m_docked; }
            set
            {
                m_docked = m_dockable ? value : false;
                m_dockTile.Image = m_docked ? m_undockImage : m_dockImage;
                if (m_docked)
                    IsRolled = false;
                Apply();
                if (m_docked && Docked != null)
                    Docked(this, new EventArgs());
                else if (!m_docked && Undocked != null)
                    Undocked(this, new EventArgs());
            }
        }
        public bool IsDockable { get { return m_dockable; } set { m_dockable = value; IsDocked = IsDocked; m_dockTile.Visible = m_dockable; } }

        #endregion



        #region Public Events.

        public event EventHandler Docked;
        public event EventHandler Undocked;
        public event EventHandler Rolled;
        public event EventHandler Unrolled;

        #endregion



        #region Construction and Destruction.

        public MetroSidePanel()
        {
            MetroSkinManager.ApplyMetroStyle(this);
            m_moveAnim = new MoveAnimation();
            Padding = new Padding(4);

            m_content = new MetroPanel();
            MetroSkinManager.ApplyMetroStyle(m_content);
            m_content.Dock = DockStyle.Fill;
            Controls.Add(m_content);

            MetroPanel titlePanel = new MetroPanel();
            MetroSkinManager.ApplyMetroStyle(titlePanel);
            titlePanel.Height = 20;
            titlePanel.Dock = DockStyle.Top;
            Controls.Add(titlePanel);

            m_titleBar = new MetroTile();
            MetroSkinManager.ApplyMetroStyle(m_titleBar);
            m_titleBar.Text = "SidePanelControler";
            m_titleBar.Height = titlePanel.Height;
            m_titleBar.Click += new EventHandler(m_titleBar_Click);
            titlePanel.Controls.Add(m_titleBar);

            m_dockImage = Bitmap.FromFile("resources/icons/appbar.pin.png");
            m_undockImage = Bitmap.FromFile("resources/icons/appbar.pin.remove.png");
            m_dockTile = new MetroTileIcon();
            MetroSkinManager.ApplyMetroStyle(m_dockTile);
            m_dockTile.Size = new Size(titlePanel.Height, titlePanel.Height);
            m_dockTile.Image = m_dockImage;
            m_dockTile.ImageAlign = ContentAlignment.MiddleCenter;
            m_dockTile.IsImageScaled = true;
            m_dockTile.ImageScale = new PointF(0.5f, 0.5f);
            m_dockTile.Click += new EventHandler(m_dockTile_Click);
            titlePanel.Controls.Add(m_dockTile);

            Load += new EventHandler(SidePanelControl_Load);
            ParentChanged += new EventHandler(SidePanelControl_ParentChanged);

            Apply();
        }

        #endregion



        #region Public Functionality.

        public void Apply()
        {
            m_moveAnim.Cancel();
            Location = CalculateTargetLocation(m_rolled, Location);

            if (m_side == DockStyle.Left)
                m_titleBar.TextAlign = ContentAlignment.MiddleRight;
            else if (m_side == DockStyle.Right)
                m_titleBar.TextAlign = ContentAlignment.MiddleLeft;
            else if (m_side == DockStyle.Bottom)
                m_titleBar.TextAlign = ContentAlignment.TopCenter;

            if (m_side == DockStyle.Left)
            {
                m_dockTile.Left = 0;
                if (m_dockable)
                {
                    m_titleBar.Left = m_dockTile.Width + 4;
                    m_titleBar.Width = Width - Padding.Horizontal - m_dockTile.Width - 4;
                }
                else
                {
                    m_titleBar.Left = 0;
                    m_titleBar.Width = Width - Padding.Horizontal;
                }
            }
            else
            {
                m_dockTile.Left = Width - Padding.Horizontal - m_dockTile.Width;
                if (m_dockable)
                {
                    m_titleBar.Left = 0;
                    m_titleBar.Width = Width - Padding.Horizontal - m_dockTile.Width - 4;
                }
                else
                {
                    m_titleBar.Left = 0;
                    m_titleBar.Width = Width - Padding.Horizontal;
                }
            }
        }

        public void Fit()
        {
            if (Parent == null)
                return;
            if (m_side == DockStyle.Left || m_side == DockStyle.Right)
                Height = Parent.Height - m_offsetPadding.Top - m_offsetPadding.Bottom;
            else if (m_side == DockStyle.Bottom)
                Width = Parent.Width - m_offsetPadding.Left - m_offsetPadding.Right;
        }

        public void Unroll()
        {
            Apply();
            m_rolled = false;
            Point pos = CalculateTargetLocation(m_rolled, Location);
            if (AnimatedRolling)
                m_moveAnim.Start(this, pos, ROLLING_TRANSITION, ROLLING_DURATION);
            else
                Location = pos;
            if (Unrolled != null)
                Unrolled(this, new EventArgs());
        }

        public void Roll()
        {
            Apply();
            m_rolled = true;
            Point pos = CalculateTargetLocation(m_rolled, Location);
            if (AnimatedRolling)
                m_moveAnim.Start(this, pos, ROLLING_TRANSITION, ROLLING_DURATION);
            else
                Location = pos;
            if (Rolled != null)
                Rolled(this, new EventArgs());
        }

        public void Toggle()
        {
            Apply();
            m_rolled = !m_rolled;
            Point pos = CalculateTargetLocation(m_rolled, Location);
            if (AnimatedRolling)
                m_moveAnim.Start(this, pos, ROLLING_TRANSITION, ROLLING_DURATION);
            else
                Location = pos;
            if (m_rolled && Rolled != null)
                Rolled(this, new EventArgs());
            else if (!m_rolled && Unrolled != null)
                Unrolled(this, new EventArgs());
        }

        #endregion



        #region Private Functionality.

        private Point CalculateTargetLocation(bool rolled, Point defaultLoc)
        {
            Size parentSize = Parent == null ? new Size() : Parent.Size;
            if (m_side == DockStyle.Left)
            {
                if (m_rolled)
                    defaultLoc = new Point(-Width + ROLLED_PART_SIZE + m_offsetPadding.Left, m_offsetPadding.Top);
                else
                    defaultLoc = new Point(m_offsetPadding.Left, m_offsetPadding.Top);
            }
            else if (m_side == DockStyle.Right)
            {
                if (m_rolled)
                    defaultLoc = new Point(parentSize.Width - ROLLED_PART_SIZE - m_offsetPadding.Right, m_offsetPadding.Top);
                else
                    defaultLoc = new Point(parentSize.Width - Width - m_offsetPadding.Right, m_offsetPadding.Top);
            }
            else if (m_side == DockStyle.Bottom)
            {
                if (m_rolled)
                    defaultLoc = new Point(m_offsetPadding.Left, parentSize.Height - ROLLED_PART_SIZE - m_offsetPadding.Bottom);
                else
                    defaultLoc = new Point(m_offsetPadding.Left, parentSize.Height - Height - m_offsetPadding.Bottom);
            }
            return defaultLoc;
        }

        #endregion



        #region Private Events Handlers.

        private void SidePanelControl_Load(object sender, EventArgs e)
        {
            Apply();
        }

        private void SidePanelControl_ParentChanged(object sender, EventArgs e)
        {
            if (m_lastParent != null)
                m_lastParent.Resize -= new EventHandler(Parent_Resize);
            if (Parent != null)
                Parent.Resize += new EventHandler(Parent_Resize);
            m_lastParent = Parent;
        }

        private void Parent_Resize(object sender, EventArgs e)
        {
            Fit();
            Apply();
        }

        private void m_titleBar_Click(object sender, EventArgs e)
        {
            if (m_docked)
                Unroll();
            else
                Toggle();
        }

        private void m_dockTile_Click(object sender, EventArgs e)
        {
            IsDocked = !IsDocked;
        }

        #endregion
    }
}
