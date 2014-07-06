using System.Drawing;
using System.Windows.Forms;
using MetroFramework.Controls;

namespace ZasuvkaPtakopyska
{
    public partial class MetroTileIcon : MetroTile
    {
        #region Public Properties.

        public Point ImageOffset { get; set; }
        
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

            if (Image == null)
                return;

            Point pos = new Point();
            if (ImageAlign == ContentAlignment.TopLeft)
                pos = new Point(0, 0);
            else if (ImageAlign == ContentAlignment.TopCenter)
                pos = new Point((Width - Image.Width) / 2, 0);
            else if (ImageAlign == ContentAlignment.TopRight)
                pos = new Point(Width - Image.Width, 0);
            else if (ImageAlign == ContentAlignment.MiddleLeft)
                pos = new Point(0, (Height - Image.Height) / 2);
            else if (ImageAlign == ContentAlignment.MiddleCenter)
                pos = new Point((Width - Image.Width) / 2, (Height - Image.Height) / 2);
            else if (ImageAlign == ContentAlignment.MiddleRight)
                pos = new Point(Width - Image.Width, (Height - Image.Height) / 2);
            else if (ImageAlign == ContentAlignment.BottomLeft)
                pos = new Point(0, Height - Image.Height);
            else if (ImageAlign == ContentAlignment.BottomCenter)
                pos = new Point((Width - Image.Width) / 2, Height - Image.Height);
            else if (ImageAlign == ContentAlignment.BottomRight)
                pos = new Point(Width - Image.Width, Height - Image.Height);

            pos.Offset(ImageOffset);
            e.Graphics.DrawImageUnscaled(Image, pos);
        }

        #endregion
    }
}
