using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using CSLib.UltraKit.Define;

namespace CSLib.UltraKit.DemoPlugin
{
    public class DemoCommand : ICommand
    {
        private string m_CmdID;

        public DemoCommand(string id)
        {
            m_CmdID = id;
        }

        /// <summary>
        ///     Decide whether this command can be reverted or not
        ///     If it can't or needn't revert, CommandManager doesn't insert it into the undo/redo command list
        /// </summary>
        public bool IsSupportUndo
        {
            get { return true; }
        }
        /// <summary>
        ///     ModuleID: which module sends command
        /// </summary>
        public int PluginID
        {
            get { return m_DemoPlugin.PlugID; }
        }

        /// <summary>
        ///     Custom Catalog, defined by user
        /// </summary>
        public string CustomCatalog
        {
            get { return "DummyCommand"; }
        }

        /// <summary>
        ///     Command Name
        /// </summary>
        public string Name
        {
            get { return "DemoCommand"; }
        }

        /// <summary>
        ///     Command Description
        /// </summary>
        public string Description
        {
            get { return "I'm a dummy command, just like fireworks to make you happy!"; }
        }

        /// <summary>
        ///     Execute the custom command
        /// </summary>
        public bool Execute()
        {
            Logger.Instance.AddLog(m_CmdID + " Executed!", LogLevel.Message, PluginID);
            return true;
        }

        /// <summary>
        ///     UnExecute the executed custom command
        /// </summary>
        public bool UnExecute()
        {
            Logger.Instance.AddLog(m_CmdID + " UnExecuted!", LogLevel.Message, PluginID);
            return true;
        }

        public void SetDemoModuleCore(DemoPlugin01 core)
        {
            m_DemoPlugin = core;
        }

        private DemoPlugin01 m_DemoPlugin = null;
    }
}
