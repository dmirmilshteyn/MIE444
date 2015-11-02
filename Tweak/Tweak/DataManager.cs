using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;
using System.Xml.Serialization;

namespace Tweak
{
    class DataManager
    {
        public Constants Load(string filePath) {
            XmlSerializer x = new XmlSerializer(typeof(Constants));
            TextReader reader = new StreamReader(filePath);
            Constants constants = (Constants)x.Deserialize(reader);
            reader.Close();

            return constants;
        }

        public void Save(string filePath, Constants constants) {
            XmlSerializer x = new XmlSerializer(typeof(Constants));
            TextWriter writer = new StreamWriter(filePath);
            x.Serialize(writer, constants);
            writer.Close();
        }
    }
}
