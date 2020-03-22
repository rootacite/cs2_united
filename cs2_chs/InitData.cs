using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Linq;
namespace cs2_chs
{
    public struct LocationData
    {
        public int X;
        public int Y;
        public int Height;
        public int Width;
    }
    public class InitData
    {
        public XDocument doc;
        public IEnumerable<XNode> atr;
        private uint addr;
        private string startUp;
        private bool enVio;
        private int vioMode;
        private bool enRep;
        private LocationData loct;

        public bool successedLoad = false;

        public LocationData Loct
        {
            get
            {
                return loct;
            }
            set
            {
                loct = value;

                doc.Root.Element("x").Value = Convert.ToString(value.X, 10);
                doc.Root.Element("y").Value = Convert.ToString(value.Y, 10);
                doc.Root.Element("w").Value = Convert.ToString(value.Width, 10);
                doc.Root.Element("h").Value = Convert.ToString(value.Height, 10);
                doc.Save("Init.xml");
            }
        }



        public bool EnRep
        {
            get
            {
                return enRep;
            }
            set
            {
                enRep = value;
                doc.Root.Element("rep").Value = value ? "true" : "false";
                doc.Save("Init.xml");
            }
        }
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
                enRep = (doc.Root.Element("rep").Value == "true" ? true : false);

                loct.X= Convert.ToInt32(doc.Root.Element("x").Value, 10);
                loct.Y = Convert.ToInt32(doc.Root.Element("y").Value, 10);
                loct.Width = Convert.ToInt32(doc.Root.Element("w").Value, 10);
                loct.Height = Convert.ToInt32(doc.Root.Element("h").Value, 10);
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
