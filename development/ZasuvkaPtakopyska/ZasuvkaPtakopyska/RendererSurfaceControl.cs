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
        private int m_isDraggingGameObject = 0;
        private int m_lastClickedGameObject = 0;
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

            Font = new Font("Verdana", 8, FontStyle.Bold);

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

            Brush brushBg = new SolidBrush(Color.FromArgb(64, 0, 0, 0));
            Brush brush = new SolidBrush(Color.White);
            float zoom = m_cameraZoom > 0.0f ? 1.0f / m_cameraZoom : 0.0f;
            string text = "Zoom: " + zoom.ToString("P2", Settings.DefaultFormatProvider);
            SizeF size = e.Graphics.MeasureString(text, Font);
            e.Graphics.FillRectangle(brushBg, 0, 0, size.Width + 20, size.Height + 20);
            e.Graphics.DrawString(text, Font, brush, new PointF(10.0f, 10.0f));
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
            PtakopyskInterface.Instance.Initialize(Handle.ToInt64());
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
            if (e.Button == MouseButtons.Left)
            {
                int handle = PtakopyskInterface.Instance.FindGameObjectHandleAtScreenPosition(e.X, e.Y, 0);
                if (handle != m_lastClickedGameObject)
                {
                    MainForm mainForm = FindForm() as MainForm;
                    if (mainForm != null)
                        mainForm.ExploreGameObjectProperties(handle, false);
                }
                m_lastClickedGameObject = handle;
                m_isDraggingGameObject = handle;
                m_lastDragPosition = e.Location;
            }
            else if (e.Button == MouseButtons.Right)
            {
                m_isDragging = true;
                m_lastCameraZoom = m_cameraZoom;
                m_lastDragPosition = e.Location;
            }
        }

        private void RendererSurfaceControl_MouseMove(object sender, MouseEventArgs e)
        {
            if (m_isDragging)
            {
                float dx = (float)(m_lastDragPosition.X - e.X);
                float dy = (float)(m_lastDragPosition.Y - e.Y);

                if (ModifierKeys.HasFlag(Keys.Control))
                {
                    float dz = 1.0f + (Math.Abs(0.01f * dy) * (ModifierKeys.HasFlag(Keys.Shift) ? 10.0f : 1.0f));
                    if (dy > 0.0f)
                        m_cameraZoom = m_lastCameraZoom * dz;
                    else if (dy < 0.0f)
                        m_cameraZoom = m_lastCameraZoom / dz;
                    else
                        m_cameraZoom = m_lastCameraZoom;
                    m_cameraZoom = Math.Max(m_cameraZoom, 0.001f);
                    PtakopyskInterface.Instance.SetSceneViewZoom(m_cameraZoom);
                    Invalidate();
                }
                else
                {
                    m_lastDragPosition = e.Location;
                    m_cameraCenter.X += dx * m_cameraZoom;
                    m_cameraCenter.Y += dy * m_cameraZoom;
                    PtakopyskInterface.Instance.SetSceneViewCenter(m_cameraCenter.X, m_cameraCenter.Y);
                    Invalidate();
                    m_lastCameraZoom = m_cameraZoom;
                }
            }
            else if (m_isDraggingGameObject != 0)
            {
                string query = "{ \"get\": { \"components\": { \"Transform\": [ \"Position\" ] } } }";
                Dictionary<string, string> result = PtakopyskInterface.Instance.QueryGameObject(m_isDraggingGameObject, false, query);
                if (result != null && result.Count > 0)
                {
                    float[] pos = null;
                    try { pos = Newtonsoft.Json.JsonConvert.DeserializeObject<float[]>(result["components/Transform/Position"]); }
                    catch { }
                    if (pos != null && pos.Length >= 2)
                    {
                        float lx, ly;
                        PtakopyskInterface.Instance.ConvertPointFromScreenToWorldSpace(m_lastDragPosition.X, m_lastDragPosition.Y, out lx, out ly);
                        m_lastDragPosition = e.Location;
                        float cx, cy;
                        PtakopyskInterface.Instance.ConvertPointFromScreenToWorldSpace(m_lastDragPosition.X, m_lastDragPosition.Y, out cx, out cy);
                        string scx = (pos[0] + (cx - lx)).ToString(Settings.DEFAULT_STRING_FORMAT, Settings.DefaultFormatProvider);
                        string scy = (pos[1] + (cy - ly)).ToString(Settings.DEFAULT_STRING_FORMAT, Settings.DefaultFormatProvider);
                        query = "{ \"set\": { \"components\": { \"Transform\": { \"Position\": [ " + scx + ", " + scy + " ] } } } }";
                        result = PtakopyskInterface.Instance.QueryGameObject(m_isDraggingGameObject, false, query);
                        Invalidate();
                        MainForm mainForm = FindForm() as MainForm;
                        if (mainForm != null)
                            mainForm.UpdateGameObjectProperties();
                    }
                }
            }
        }

        private void RendererSurfaceControl_MouseUp(object sender, MouseEventArgs e)
        {
            m_isDragging = false;
            m_isDraggingGameObject = 0;
            m_lastCameraZoom = m_cameraZoom;
            m_lastDragPosition = e.Location;
        }

        #endregion
    }
}
