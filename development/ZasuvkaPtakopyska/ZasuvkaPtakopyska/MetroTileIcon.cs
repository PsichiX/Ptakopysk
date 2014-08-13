using System.Drawing;
using System.Windows.Forms;
using MetroFramework.Controls;
using MetroFramework;

namespace ZasuvkaPtakopyska
{
    public partial class MetroTileIcon : MetroTile
    {
        #region Public Properties.

        public Point ImageOffset { get; set; }
        public bool IsImageScaled { get; set; }
        public PointF ImageScale { get; set; }

        #endregion



        #region Construction and Destruction.

        public MetroTileIcon()
        {
            ImageAlign = ContentAlignment.MiddleCenter;
        }

        #endregion



        #region Protected Functionality.

        protected override void OnPaint(PaintEventArgs e)
        {
            base.OnPaint(e);

            if (Image != null)
            {
                Size size = IsImageScaled ? new Size((int)(Image.Width * ImageScale.X), (int)(Image.Height * ImageScale.Y)) : Image.Size;
                Point pos = new Point();
                if (ImageAlign == ContentAlignment.TopLeft)
                    pos = new Point(0, 0);
                else if (ImageAlign == ContentAlignment.TopCenter)
                    pos = new Point((Width - size.Width) / 2, 0);
                else if (ImageAlign == ContentAlignment.TopRight)
                    pos = new Point(Width - size.Width, 0);
                else if (ImageAlign == ContentAlignment.MiddleLeft)
                    pos = new Point(0, (Height - size.Height) / 2);
                else if (ImageAlign == ContentAlignment.MiddleCenter)
                    pos = new Point((Width - size.Width) / 2, (Height - size.Height) / 2);
                else if (ImageAlign == ContentAlignment.MiddleRight)
                    pos = new Point(Width - size.Width, (Height - size.Height) / 2);
                else if (ImageAlign == ContentAlignment.BottomLeft)
                    pos = new Point(0, Height - size.Height);
                else if (ImageAlign == ContentAlignment.BottomCenter)
                    pos = new Point((Width - size.Width) / 2, Height - size.Height);
                else if (ImageAlign == ContentAlignment.BottomRight)
                    pos = new Point(Width - size.Width, Height - size.Height);

                pos.Offset(ImageOffset);
                if (IsImageScaled)
                    e.Graphics.DrawImage(Image, pos.X, pos.Y, size.Width, size.Height);
                else
                    e.Graphics.DrawImageUnscaled(Image, pos);
            }

            if (Focused)
            {
                Color c = Color.Gray;
                if (Theme == MetroThemeStyle.Dark)
                    c = Color.White;
                else if (Theme == MetroThemeStyle.Light)
                    c = Color.Black;
                Pen pen = new Pen(c, 3);
                e.Graphics.DrawRectangle(pen, 1, 1, Width - 3, Height - 3);
            }
        }

        #endregion
    }
}
