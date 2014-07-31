using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace ZasuvkaPtakopyska
{
    public class PtakopyskSceneRenderer : UserControl
    {
        #region Construction and Destruction.

        public PtakopyskSceneRenderer()
        {
            Load += new EventHandler(PtakopyskSceneRenderer_Load);
            Disposed += new EventHandler(PtakopyskSceneRenderer_Disposed);
        }

        #endregion



        #region Protected Functionality.

        protected override void OnPaint(PaintEventArgs e)
        {
            PtakopyskInterface.ProcessEvents();
            PtakopyskInterface.ProcessRender();
        }

        protected override void OnPaintBackground(PaintEventArgs e)
        {
        }
        
        #endregion



        #region Private Events Handlers.

        private void PtakopyskSceneRenderer_Load(object sender, EventArgs e)
        {
            PtakopyskInterface.Initialize(Handle);
        }

        private void PtakopyskSceneRenderer_Disposed(object sender, EventArgs e)
        {
            PtakopyskInterface.Release();
        }

        #endregion
    }
}
