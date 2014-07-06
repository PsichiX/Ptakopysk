using System;
using System.Drawing;
using System.Windows.Forms;
using MetroFramework.Controls;
using MetroFramework.Animation;
using MetroFramework.Components;

namespace ZasuvkaPtakopyska
{
    public partial class MetroSidePanel : MetroUserControl
    {
        #region Private Static Data.

        private static readonly int ROLLED_PART_SIZE = 24;
        private static readonly int ROLLING_DURATION = 15;
        private static readonly TransitionType ROLLING_TRANSITION = TransitionType.EaseInExpo;

        #endregion



        #region Private Data.

        private MetroTile m_titleBar;
        private MetroPanel m_content;
        private DockStyle m_side = DockStyle.None;
        private bool m_rolled = false;
        private Padding m_offsetPadding;
        private MoveAnimation m_moveAnim;
        private Control m_lastParent;

        #endregion



        #region Public Properties

        public override string Text { get { return m_titleBar.Text; } set { m_titleBar.Text = value; } }
        public MetroPanel Content { get { return m_content; } }
        public DockStyle Side { get { return m_side; } set { m_side = value; Apply(); } }
        public bool IsRolled { get { return m_rolled; } set { m_rolled = value; if (m_rolled) Roll(); else Unroll(); } }
        public bool AnimatedRolling { get; set; }
        public Padding OffsetPadding { get { return m_offsetPadding; } set { m_offsetPadding = value; Apply(); } }

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

            m_titleBar = new MetroTile();
            MetroSkinManager.ApplyMetroStyle(m_titleBar);
            m_titleBar.Text = "SidePanelControler";
            m_titleBar.Height = 20;
            m_titleBar.Dock = DockStyle.Top;
            m_titleBar.Click += new EventHandler(m_titleBar_Click);
            Controls.Add(m_titleBar);

            Load += new EventHandler(SidePanelControl_Load);
            ParentChanged += new EventHandler(SidePanelControl_ParentChanged);
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
            Toggle();
        }

        #endregion
    }
}
