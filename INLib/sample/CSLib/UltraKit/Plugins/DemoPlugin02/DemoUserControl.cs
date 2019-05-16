using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using CSLib.UltraKit.Define;

namespace CSLib.UltraKit.DemoPlugin
{
    public partial class DemoUserControl : UserControl, IUCMenu
    {
        public DemoUserControl()
        {
            InitializeComponent();
        }

        private DemoPlugin02 m_DemoPlugin;

        public MenuStrip PluginMenu
        {
            get
            {
                return PluginMenuStrip;
            }
        }

        public void RemoveMenu()
        {
            this.Controls.Remove(PluginMenuStrip);
        }

        public void SetDemoModuleCore(DemoPlugin02 core)
        {
            m_DemoPlugin = core;
        }

        public void SetEventMessage(string msg)
        {
            textBoxMsg.Text = msg;
        }

        private void SendMsg()
        {
            UltraKitEvent testEvent = new UltraKitEvent();
            testEvent.SenderID = m_DemoPlugin.PlugID;
            testEvent.EventName = "ULTRAKIT_PLUGIN_EVENT_TEST_01";
            testEvent.CustomData.Add("Hello, I'm plug02!");
            EventManager.Instance.SendEvent(testEvent);
        }
        
        private void buttonSayHello_Click(object sender, EventArgs e)
        {
            MessageBox.Show("Hello DemoPlugin02");
        }

        private void button2_Click(object sender, EventArgs e)
        {
            DemoCommand cmd = new DemoCommand("Demo02UnExecCmd");
            cmd.SetDemoModuleCore(m_DemoPlugin);

            CommandManager.Instance.Insert(cmd);
        }

        private void button3_Click(object sender, EventArgs e)
        {
            DemoCommand cmd = new DemoCommand("Demo02ExecCmd");
            cmd.SetDemoModuleCore(m_DemoPlugin);
            cmd.Execute();
            CommandManager.Instance.InsertIntoUndoList(cmd);
        }

        private void buttonSendEvent_Click(object sender, EventArgs e)
        {
            // Send Event
            //DemoEvent dEvent = new DemoEvent();
            SendMsg();
            //m_fmwkCoreRef.SendEvent(dEvent);
        }

        private void buttonClear_Click(object sender, EventArgs e)
        {
            textBoxMsg.Text = "No Msg";
        }

        private void sayHelloToolStripMenuItem_Click(object sender, EventArgs e)
        {
            MessageBox.Show("Hello DemoPlugin02");
        }
    }
}
