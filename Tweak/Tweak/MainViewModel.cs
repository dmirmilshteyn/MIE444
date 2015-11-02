using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;

namespace Tweak
{
    class MainViewModel : ObservableObject
    {
        Constants constants;
        public Constants Constants {
            get { return constants; }
            set {
                constants = value;
                RaisePropertyChanged();
            }
        }

        public ICommand LoadDataFileCommand { get; private set; }
        public ICommand SaveDataFileCommand { get; private set; }

        public MainViewModel() {
            Constants = new Constants();

            this.LoadDataFileCommand = new Command(LoadDataFileCallback);
            this.SaveDataFileCommand = new Command(SaveDataFileCallback);
        }

        private void LoadDataFileCallback() {
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.Filter = "XML Files|*.xml";
            bool? result = ofd.ShowDialog();
            if (result.HasValue && result.Value) {
                string path = ofd.FileName;

                DataManager dataManager = new DataManager();
                Constants = dataManager.Load(path);
            }
        }

        private void SaveDataFileCallback() {
            SaveFileDialog sfd = new SaveFileDialog();
            sfd.Filter = "XML Files|*.xml";
            sfd.AddExtension = true;
            bool? result = sfd.ShowDialog();
            if (result.HasValue && result.Value) {
                string path = sfd.FileName;

                DataManager dataManager = new DataManager();
                dataManager.Save(path, Constants);
            }
        }
    }
}
