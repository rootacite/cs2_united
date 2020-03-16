using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Linq;
namespace cs2_chs
{
    public class InitData
    {
        public XDocument doc;
        public IEnumerable<XNode> atr;
        private uint addr;
        private string startUp;
        private bool enVio;
        private int vioMode;

        public bool successedLoad = false;
        public int VioMode
        {
            get
            {
                return vioMode;
            }
            set
            {
                vioMode = value;
                doc.Root.Element("viom").Value = Convert.ToString(value, 10);
                doc.Save("Init.xml");
            }
        }
        public bool Envio
        {
            get
            {
                return enVio;
            }
            set
            {
                enVio = value;
                doc.Root.Element("vio").Value = value ? "true" : "false";
                doc.Save("Init.xml");
            }
        }
        public string StartUp {
            get
            {
                return startUp;
            }
            set
            {
                startUp = value;
                doc.Root.Element("StartUp").Value = value;
                doc.Save("Init.xml");
            }
        }
        public uint Addr
        {
            get
            {
                return addr;
            }
            set
            {
                addr = value;
                doc.Root.Element("Addr").Value = Convert.ToString(value, 16);
                doc.Save("Init.xml");
            }
        }
        public InitData()
        {
            try
            {
                doc = XDocument.Load("Init.xml");
                atr = doc.Root.Nodes();
                startUp = doc.Root.Element("StartUp").Value;
                addr = Convert.ToUInt32(doc.Root.Element("Addr").Value, 16);
                enVio = (doc.Root.Element("vio").Value == "true" ? true : false);
                vioMode = Convert.ToInt32(doc.Root.Element("viom").Value, 10);
            }
            catch (Exception e)
            {
                successedLoad = false;
                return;
            }
            successedLoad = true;
       
        }
        ~InitData()
        {
           
        }
    }
}
