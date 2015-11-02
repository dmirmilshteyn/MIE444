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
        public Project Load(string filePath) {
            XmlSerializer x = new XmlSerializer(typeof(Project));
            TextReader reader = new StreamReader(filePath);
            Project project = (Project)x.Deserialize(reader);
            reader.Close();

            return project;
        }

        public void Save(string filePath, Project project) {
            XmlSerializer x = new XmlSerializer(typeof(Project));
            TextWriter writer = new StreamWriter(filePath);
            x.Serialize(writer, project);
            writer.Close();
        }
    }
}
