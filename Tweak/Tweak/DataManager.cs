using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;
using System.Xml.Serialization;
using Windows.Storage;

namespace Tweak
{
    class DataManager
    {
        public async Task<Project> Load(StorageFile storageFile) {
            XmlSerializer x = new XmlSerializer(typeof(Project));
            using (Stream fileStream = await storageFile.OpenStreamForReadAsync()) {
                return (Project)x.Deserialize(fileStream);
            }
        }

        public async Task Save(StorageFile storageFile, Project project) {
            XmlSerializer x = new XmlSerializer(typeof(Project));
            using (Stream fileStream = await storageFile.OpenStreamForWriteAsync()) {
                x.Serialize(fileStream, project);
            }
        }
    }
}
