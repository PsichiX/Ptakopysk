using System;
using System.Windows.Forms;
using System.Drawing;
using ZasuvkaPtakopyskaExtender;
using System.Collections.Generic;

namespace ZasuvkaPtakopyska
{
    public class RendererSurfaceControl : UserControl
    {
        #region Private Data.

        private Timer m_timer;
        private bool m_isDragging = false;
        private Point m_lastDragPosition;
        private PointF m_cameraCenter;
        private float m_cameraZoom = 1.0f;
        private float m_lastCameraZoom = 1.0f;

        #endregion



        #region Construction and Destruction.

        public RendererSurfaceControl()
        {
            Load += new EventHandler(RendererSurfaceControl_Load);
            Disposed += new EventHandler(RendererSurfaceControl_Disposed);
            Resize += new EventHandler(RendererSurfaceControl_Resize);
            MouseDown += new MouseEventHandler(RendererSurfaceControl_MouseDown);
            MouseMove += new MouseEventHandler(RendererSurfaceControl_MouseMove);
            MouseUp += new MouseEventHandler(RendererSurfaceControl_MouseUp);

            PtakopyskInterface.Instance.SetSceneViewZoom(m_cameraZoom);
            PtakopyskInterface.Instance.SetSceneViewCenter(m_cameraCenter.X, m_cameraCenter.Y);
            
            m_timer = new Timer();
            m_timer.Interval = 1000 / 20;
            m_timer.Enabled = false;
            m_timer.Tick += new EventHandler(m_timer_Tick);
        }

        #endregion



        #region Protected Functionality.

        protected override void OnPaint(PaintEventArgs e)
        {
            PtakopyskInterface.Instance.ProcessEvents();
            PtakopyskInterface.Instance.ProcessUpdate(0, true);
            PtakopyskInterface.Instance.ProcessRender(false);
        }

        protected override void OnPaintBackground(PaintEventArgs pevent)
        {
        }

        protected override bool ProcessCmdKey(ref Message msg, Keys keyData)
        {
            return base.ProcessCmdKey(ref msg, keyData);
        }

        #endregion



        #region Public Functionality.

        public void RebuildSceneView(string json)
        {
            PtakopyskInterface.Instance.ClearScene();
            if (!string.IsNullOrEmpty(json))
                PtakopyskInterface.Instance.ApplyJsonToScene(json);
            Invalidate();
        }

        #endregion



        #region Private Events Handlers.

        private void RendererSurfaceControl_Load(object sender, EventArgs e)
        {
            PtakopyskInterface.Instance.Initialize(
                Handle.ToInt32(),
                "",//"resources/textures/logo.png",
                "",//"resources/shaders/color-texture.vs",
                "",//"resources/shaders/color-texture.fs",
                ""//"resources/fonts/verdana.ttf"
                );
            PtakopyskInterface.Instance.SetVerticalSyncEnabled(false);
        }

        private void RendererSurfaceControl_Disposed(object sender, EventArgs e)
        {
            PtakopyskInterface.Instance.Release();
        }

        private void RendererSurfaceControl_Resize(object sender, EventArgs e)
        {
            PtakopyskInterface.Instance.SetSceneViewSize(Width, Height);
        }

        private void m_timer_Tick(object sender, EventArgs e)
        {
            Invalidate();
        }

        private void RendererSurfaceControl_MouseDown(object sender, MouseEventArgs e)
        {
            if (e.Button != MouseButtons.Right)
                return;

            m_isDragging = true;
            m_lastCameraZoom = m_cameraZoom;
            m_lastDragPosition = e.Location;
        }

        private void RendererSurfaceControl_MouseMove(object sender, MouseEventArgs e)
        {
            if (!m_isDragging)
                return;

            float dx = (float)(m_lastDragPosition.X - e.X);
            float dy = (float)(m_lastDragPosition.Y - e.Y);

            if (ModifierKeys.HasFlag(Keys.Control))
            {
                m_cameraZoom = m_lastCameraZoom + (0.01f * dy);
                PtakopyskInterface.Instance.SetSceneViewZoom(m_cameraZoom);
                Invalidate();
            }
            else
            {
                m_lastDragPosition = e.Location;
                m_cameraCenter.X += dx;
                m_cameraCenter.Y += dy;
                PtakopyskInterface.Instance.SetSceneViewCenter(m_cameraCenter.X, m_cameraCenter.Y);
                Invalidate();
                m_lastCameraZoom = m_cameraZoom;
            }
        }

        private void RendererSurfaceControl_MouseUp(object sender, MouseEventArgs e)
        {
            m_isDragging = false;
            m_lastCameraZoom = m_cameraZoom;
            m_lastDragPosition = e.Location;
        }

        #endregion
    }
}
