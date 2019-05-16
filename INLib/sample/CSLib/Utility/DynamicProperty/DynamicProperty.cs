using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Reflection;
using CSLib.Utility;

namespace DynamicProperty
{
    public partial class DynamicProperty : Form
    {
        public DynamicProperty()
        {
            InitializeComponent();
        }

        private void BrowseTypeComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            m_Browses.TestDynamicAttribute = (ETestAttribute)BrowseTypeComboBox.SelectedIndex;
            PropertyDescriptorCollection descs = TypeDescriptor.GetProperties(m_Browses);
            if (descs!=null)
            {
                BrowseTypeTextBox.Text = "";
                foreach (PropertyDescriptor pd in descs)
                {
                    CPropertyDescriptor proDesc = pd as CPropertyDescriptor;
                    if (proDesc!=null)
                    {
                        BrowseTypeTextBox.Text += proDesc.GetValue(null).ToString() + "\r\n";
                    }                    
                }
            }
        }

        //
        private Browses m_Browses = new Browses();
    }
}
