using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;
using System.Reflection;

namespace DynamicProperty
{
    public enum ETestAttribute
    {
        ETestAttribute1,
        ETestAttribute2
    }

    [AttributeUsage(AttributeTargets.Property)]
    public class DynamicSelector : Attribute
    {
    }

    public class CDynamicPropertyClass
    {
        public CDynamicPropertyClass()
        {
        }

        public void GetDynamicProperties(CSLib.Utility.CPropertyDescriptorList pdl)
        {
            pdl.Clear();

            PropertyInfo[] proInfo = GetType().GetProperties();
            foreach (PropertyInfo p in proInfo)
            {
                Attribute[] dyna = new Attribute[0];
                if (m_attriActiveType != null)
                {
                    dyna = (Attribute[])p.GetCustomAttributes(m_attriActiveType, true);
                }

                if (dyna.Length == 1)
                {
                    CSLib.Utility.CPropertyDescriptor proDesc = new CSLib.Utility.CPropertyDescriptor(this, p.GetValue(this, null), p.Name, p.GetValue(this, null).GetType(), null, dyna[0]);
                    pdl.Add(proDesc);
                }
            }
        }

        public Type AttriActiveType
        {
            set { m_attriActiveType = value; }
        }

        private Type m_attriActiveType = typeof(Browses.BrowsesType1);
    }

    [TypeDescriptionProvider(typeof(CSLib.Utility.CTypeDescriptionProvider))]
    public class Browses : CDynamicPropertyClass
    {
        public Browses()
        {
        }

        public class BrowsesType1 : Attribute
        {
        }

        public class BrowsesType2 : Attribute
        {
        }

        [BrowsesType1]
        public string IE
        {
            get { return m_IE; }
            set { m_IE = value; }
        }

        [BrowsesType2]
        public string Firefox
        {
            get { return m_Firefox; }
            set { m_Firefox = value; }
        }

        [BrowsesType2]
        public string Chrome
        {
            get { return m_Chrome; }
            set { m_Chrome = value; }
        }

        [DynamicSelector]
        public ETestAttribute TestDynamicAttribute
        {
            get { return m_eTestAttribute; }
            set
            {
                m_eTestAttribute = value;

                switch (m_eTestAttribute)
                {
                    case ETestAttribute.ETestAttribute1:
                        AttriActiveType = typeof(BrowsesType1);
                        break;
                    case ETestAttribute.ETestAttribute2:
                        AttriActiveType = typeof(BrowsesType2);
                        break;
                    default:
                        break;
                }
            }
        }

        //
        protected string m_IE = "ie";
        protected string m_Firefox = "firefox";
        protected string m_Chrome = "chrome";

        protected ETestAttribute m_eTestAttribute = ETestAttribute.ETestAttribute1;
    }
}
