using System;
using System.Drawing;
using System.Windows.Forms;
using MetroFramework.Controls;
using MetroFramework.Animation;
using MetroFramework.Components;
using ZasuvkaPtakopyskaExtender;
using System.Collections.Generic;

namespace ZasuvkaPtakopyska
{
    public partial class MetroSidePanel : MetroUserControl
    {
        #region Nested Interfaces.

        public interface IMetroSidePanelScrollableContent
        {
            MetroSidePanel SidePanel { get; set; }
            Rectangle ScrollableContentRectangle { get; }
            int VerticalScrollValue { get; set; }
            int VerticalScrollMaximum { get; set; }
            int VerticalScrollLargeChange { get; set; }
            int HorizontalScrollValue { get; set; }
            int HorizontalScrollMaximum { get; set; }
            int HorizontalScrollLargeChange { get; set; }
        }

        #endregion



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
        private MetroScrollBar m_contentScrollbarV;
        private MetroScrollBar m_contentScrollbarH;
        private DockStyle m_side = DockStyle.None;
        private bool m_rolled = false;
        private Padding m_offsetPadding;
        private bool m_docked = false;
        private bool m_dockable = true;
        private MoveAnimation m_moveAnim;
        private Control m_lastParent;
        private List<IMetroSidePanelScrollableContent> m_scrollableControls = new List<IMetroSidePanelScrollableContent>();

        #endregion



        #region Public Properties

        public override string Text { get { return m_titleBar.Text; } set { m_titleBar.Text = value; } }
        public MetroPanel Content { get { return m_content; } }
        public DockStyle Side { get { return m_side; } set { m_side = value; Fit(); Apply(); } }
        public bool IsRolled { get { return m_rolled; } set { m_rolled = value; if (!m_docked && m_rolled) Roll(); else Unroll(); } }
        public bool AnimatedRolling { get; set; }
        public Padding OffsetPadding { get { return m_offsetPadding; } set { m_offsetPadding = value; Fit(); Apply(); } }
        public bool IsDocked
        {
            get { return m_docked; }
            set
            {
                m_docked = m_dockable ? value : false;
                m_dockTile.Image = m_docked ? m_undockImage : m_dockImage;
                if (m_docked)
                    IsRolled = false;
                Fit();
                Apply();
                if (m_docked && Docked != null)
                    Docked(this, new EventArgs());
                else if (!m_docked && Undocked != null)
                    Undocked(this, new EventArgs());
            }
        }
        public bool IsDockable { get { return m_dockable; } set { m_dockable = value; IsDocked = IsDocked; m_dockTile.Visible = m_dockable; } }
        public MetroScrollBar ScrollbarV { get { return m_contentScrollbarV; } }

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

            MetroPanel contentPanel = new MetroPanel();
            MetroSkinManager.ApplyMetroStyle(contentPanel);
            contentPanel.Dock = DockStyle.Fill;
            Controls.Add(contentPanel);

            m_content = new MetroPanel();
            MetroSkinManager.ApplyMetroStyle(m_content);
            m_content.Dock = DockStyle.Fill;
            m_content.Controls.Clear();
            m_content.ControlAdded += new ControlEventHandler(m_content_ControlAdded);
            m_content.ControlRemoved += new ControlEventHandler(m_content_ControlRemoved);
            contentPanel.Controls.Add(m_content);

            m_contentScrollbarV = new MetroScrollBar(MetroScrollOrientation.Vertical);
            MetroSkinManager.ApplyMetroStyle(m_contentScrollbarV);
            m_contentScrollbarV.Dock = DockStyle.Right;
            m_contentScrollbarV.Scroll += new ScrollEventHandler(m_contentScrollbarV_Scroll);
            contentPanel.Controls.Add(m_contentScrollbarV);

            m_contentScrollbarH = new MetroScrollBar(MetroScrollOrientation.Horizontal);
            MetroSkinManager.ApplyMetroStyle(m_contentScrollbarH);
            m_contentScrollbarH.Dock = DockStyle.Bottom;
            m_contentScrollbarH.Scroll += new ScrollEventHandler(m_contentScrollbarH_Scroll);
            contentPanel.Controls.Add(m_contentScrollbarH);

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

            Fit();
            Apply();
            UpdateScrollbars();
        }

        #endregion



        #region Public Functionality.

        public void UpdateScrollbars()
        {
            if (m_content.Controls.Count == 0)
            {
                m_contentScrollbarV.Maximum = 1;
                m_contentScrollbarV.LargeChange = 1;
                m_contentScrollbarV.Visible = false;
                m_contentScrollbarH.Maximum = 1;
                m_contentScrollbarH.LargeChange = 1;
                m_contentScrollbarH.Visible = false;
            }
            else
            {
                Rectangle rect;
                m_content.CalculateContentsRectangle(
                    out rect,
                    (c, r) => (c is IMetroSidePanelScrollableContent ? (c as IMetroSidePanelScrollableContent).ScrollableContentRectangle : r)
                    );
                m_contentScrollbarV.Maximum = rect.Height;
                m_contentScrollbarV.LargeChange = m_content.Height;
                m_contentScrollbarV.Visible = m_contentScrollbarV.Maximum > m_contentScrollbarV.LargeChange;
                m_contentScrollbarH.Maximum = rect.Width;
                m_contentScrollbarH.LargeChange = m_content.Width;
                m_contentScrollbarH.Visible = m_contentScrollbarH.Maximum > m_contentScrollbarH.LargeChange;
            }
            m_content.VerticalScroll.Maximum = m_contentScrollbarV.Maximum;
            m_content.VerticalScroll.LargeChange = m_contentScrollbarV.LargeChange;
            m_content.HorizontalScroll.Maximum = m_contentScrollbarH.Maximum;
            m_content.HorizontalScroll.LargeChange = m_contentScrollbarH.LargeChange;
            foreach (IMetroSidePanelScrollableContent c in m_scrollableControls)
            {
                c.VerticalScrollMaximum = m_contentScrollbarV.Maximum;
                c.VerticalScrollLargeChange = m_contentScrollbarV.LargeChange;
                c.HorizontalScrollMaximum = m_contentScrollbarH.Maximum;
                c.HorizontalScrollLargeChange = m_contentScrollbarH.LargeChange;
            }
        }

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
            Fit();
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
            Fit();
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
            Fit();
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

        private void m_contentScrollbarV_Scroll(object sender, ScrollEventArgs e)
        {
            m_content.VerticalScroll.Value = e.NewValue;
            foreach (IMetroSidePanelScrollableContent c in m_scrollableControls)
                c.VerticalScrollValue = e.NewValue;
        }

        private void m_contentScrollbarH_Scroll(object sender, ScrollEventArgs e)
        {
            m_content.HorizontalScroll.Value = e.NewValue;
            foreach (IMetroSidePanelScrollableContent c in m_scrollableControls)
                c.HorizontalScrollValue = e.NewValue;
        }

        private void m_content_ControlAdded(object sender, ControlEventArgs e)
        {
            IMetroSidePanelScrollableContent c = e.Control as IMetroSidePanelScrollableContent;
            if (c != null && !m_scrollableControls.Contains(c))
            {
                m_scrollableControls.Add(c);
                c.SidePanel = this;
            }
            UpdateScrollbars();
            m_contentScrollbarV.Value = m_contentScrollbarV.Minimum;
            m_contentScrollbarH.Value = m_contentScrollbarH.Minimum;
        }

        private void m_content_ControlRemoved(object sender, ControlEventArgs e)
        {
            IMetroSidePanelScrollableContent c = e.Control as IMetroSidePanelScrollableContent;
            if (c != null && m_scrollableControls.Contains(c))
            {
                m_scrollableControls.Remove(c);
                c.SidePanel = null;
            }
            UpdateScrollbars();
            m_contentScrollbarV.Value = m_contentScrollbarV.Minimum;
            m_contentScrollbarH.Value = m_contentScrollbarH.Minimum;
        }

        #endregion
    }
}
