using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.Windows.Forms;
using CSLib.UltraKit.Define;

namespace CSLib.UltraKit.DemoPlugin
{
    public class DemoPlugin01 : IPlugIn, IEvent
    {
        #region IPlugIn Interface

        /// <summary>
        ///     UnInitialize the resources
        /// </summary>
        public void Dispose()
        {
        }

        public int PlugID 
        {
            get { return m_pluginId; } 
        }

        public void InitComponent(IFramework framework, int pluginId)
        {
            if (framework == null)
            {
                return;
            }
            m_Framework = framework;
            m_pluginId = pluginId;

            // Init UserControl
            if (m_UserControl == null)
            {
                m_UserControl = new DemoUserControl();
            }
            
            m_UserControl.SetDemoModuleCore(this);
        }

        public void AttachPluginUserControl()
        {
            if (m_Framework != null)
            {
                m_Framework.AttachPluginUserControl(m_pluginId, m_UserControl);
                m_Framework.RegisterToolbarMenu(m_pluginId, m_UserControl);
            } 
        }

        public void DetachPluginUserControl()
        {
            if (m_Framework != null)
            {
                m_Framework.DetachPluginUserControl(m_pluginId, m_UserControl);
            } 
        }

        #endregion

        #region IEvent Interface
        public void RegistEvents()
        {
            EventManager.Instance.RegisterEvent("ULTRAKIT_PLUGIN_EVENT_TEST_01", m_pluginId, new OnUltraKitEvent(Plugin01Event));
        }

        public void UnRegistEvents()
        {
            EventManager.Instance.UnRegisterEvent("ULTRAKIT_PLUGIN_EVENT_TEST_01", m_pluginId);
        }

        private void Plugin01Event(UltraKitEvent e)
        {
            string sendPluginName = "Unknow";
            string receivePluginName = "Unknow";
            PlugInAttributes pa = PlugInContext.Instance.GetPlugIn(e.SenderID);
            if (pa != null)
            {
                sendPluginName = pa.PluginName;
            }
            pa = PlugInContext.Instance.GetPlugIn(m_pluginId);
            if (pa != null)
            {
                receivePluginName = pa.PluginName;
            }
            string customData = "";
            foreach (string data in e.CustomData)
            {
                customData += data;
            }
            string msg = string.Format("Send PluginName: {0} \r\nEventName: {1}\r\nReceive PluginName: {2}\r\nData: {3}.", sendPluginName, e.EventName, receivePluginName, customData);
            m_UserControl.SetEventMessage(msg);
        }

        #endregion

        #region  IKeyboardMouseInputObserver Interface
        /// <summary>
        ///     Indicates this observer booked events
        /// </summary>
        public InputEventType BookedInputEvent
        {
            get { return InputEventType.KeyDown; }
        }

        /// <summary>
        ///     Indicates this observer type
        /// </summary>
        public ObserverType ObserverType
        {
            get { return ObserverType.ModuleObserver; }
        }

        /// <summary>
        ///     Indicates this observer sub type: which module's observer
        /// </summary>
        public EditorPluginID ObserverSubType
        {
            get { return EditorPluginID.PLUGIN_UNKNOWN_ID; }
        }

        /// <summary>
        ///     Observer name
        /// </summary>
        public string ObserverName
        {
            get { return "DemoModule"; }
        }

        public bool Update(KeyboardMouseInput kmInput)
        {
            if (kmInput.KeyDown[(int)Keys.Enter])
            {
                Logger.Instance.AddLog("Enter Key Down", LogLevel.Message, (int)EditorPluginID.PLUGIN_UNKNOWN_ID);
                return true;
            }
            return false;
        }
        #endregion

        /// <summary>
        ///     Here, just for sample!!!
        /// </summary>
        public IFramework FrameworkObj
        {
            get { return m_Framework; }
        }
        
        //
        private IFramework m_Framework = null;
        private DemoUserControl m_UserControl = null;
        private int m_pluginId = -1;
    }
}
