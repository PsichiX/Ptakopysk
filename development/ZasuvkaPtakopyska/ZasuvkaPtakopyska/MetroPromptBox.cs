using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using MetroFramework;
using MetroFramework.Forms;
using System.Drawing;
using MetroFramework.Controls;
using System.Windows.Forms;

namespace ZasuvkaPtakopyska
{
    public class MetroPromptBox : MetroForm
    {
        #region Private Static Data.

        private static readonly int DEFAULT_SEPARATOR = 16;
        private static readonly int DEFAULT_BUTTON_HEIGHT = 24;
        private static readonly Size DEFAULT_SIZE = new Size(640, 320);
        
        #endregion



        #region Private Data.

        private MetroLabel m_messageLabel;
        private MetroTextBox m_valueTextBox;
        private MetroPanel m_buttonsPanel;
        #endregion



        #region Public Properties.

        public string Title { get { return Text; } set { Text = value; } }
        public string Message { get { return m_messageLabel.Text; } set { m_messageLabel.Text = value; UpdateLayout(); } }
        public string Value { get { return m_valueTextBox.Text; } set { m_valueTextBox.Text = value; } }

        #endregion



        #region Construction and Destruction.

        public MetroPromptBox()
        {
            MetroSkinManager.ApplyMetroStyle(this);
            Size = DEFAULT_SIZE;
            Padding = new Padding(DEFAULT_SEPARATOR, 0, DEFAULT_SEPARATOR, DEFAULT_SEPARATOR);
            ControlBox = false;
            ShowInTaskbar = false;
            Resizable = false;
            DialogResult = DialogResult.None;

            m_buttonsPanel = new MetroPanel();
            MetroSkinManager.ApplyMetroStyle(m_buttonsPanel);
            m_buttonsPanel.Height = DEFAULT_BUTTON_HEIGHT + DEFAULT_SEPARATOR;
            m_buttonsPanel.Padding = new Padding(0, DEFAULT_SEPARATOR, 0, 0);
            m_buttonsPanel.Dock = DockStyle.Top;
            Controls.Add(m_buttonsPanel);

            MetroButton button = new MetroButton();
            MetroSkinManager.ApplyMetroStyle(button);
            button.Text = "&OK";
            button.Tag = DialogResult.OK;
            button.Dock = DockStyle.Left;
            button.Click += new EventHandler(button_Click);
            m_buttonsPanel.Controls.Add(button);

            button = new MetroButton();
            MetroSkinManager.ApplyMetroStyle(button);
            button.Text = "&Cancel";
            button.Tag = DialogResult.Cancel;
            button.Dock = DockStyle.Right;
            button.Click += new EventHandler(button_Click);
            m_buttonsPanel.Controls.Add(button);

            m_valueTextBox = new MetroTextBox();
            MetroSkinManager.ApplyMetroStyle(m_valueTextBox);
            m_valueTextBox.Text = "";
            m_valueTextBox.Dock = DockStyle.Top;
            Controls.Add(m_valueTextBox);

            m_messageLabel = new MetroLabel();
            MetroSkinManager.ApplyMetroStyle(m_messageLabel);
            m_messageLabel.Text = "";
            m_messageLabel.Dock = DockStyle.Top;
            Controls.Add(m_messageLabel);

            UpdateLayout();
        }

        private void button_Click(object sender, EventArgs e)
        {
            MetroButton button = sender as MetroButton;
            if (button == null)
                return;

            if (button.Tag is DialogResult)
                DialogResult = (DialogResult)button.Tag;
            Close();
        }

        #endregion



        #region Private Functionality.

        private void UpdateLayout()
        {
            Height = m_buttonsPanel.Bottom + DEFAULT_SEPARATOR;
        }
        
        #endregion
    }
}
