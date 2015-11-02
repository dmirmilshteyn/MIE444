using Microsoft.Win32;
using Microsoft.WindowsAPICodePack.Dialogs;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;

namespace Tweak
{
    class MainViewModel : ObservableObject
    {
        Project project;
        public Project Project {
            get { return project; }
            set {
                project = value;
                RaisePropertyChanged();
            }
        }

        public ICommand LoadDataFileCommand { get; private set; }
        public ICommand SaveDataFileCommand { get; private set; }
        public ICommand BrowseOutputDirectoryCommand { get; private set; }
        public ICommand GenerateCodeCommand { get; private set; }

        public MainViewModel() {
            Project = new Project();

            this.LoadDataFileCommand = new Command(LoadDataFileCallback);
            this.SaveDataFileCommand = new Command(SaveDataFileCallback);
            this.BrowseOutputDirectoryCommand = new Command(BrowseOutputDirectoryCallback);
            this.GenerateCodeCommand = new Command(GenerateCodeCallback);
        }

        private void LoadDataFileCallback() {
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.Filter = "XML Files|*.xml";
            bool? result = ofd.ShowDialog();
            if (result.HasValue && result.Value) {
                string path = ofd.FileName;

                DataManager dataManager = new DataManager();
                Project = dataManager.Load(path);
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
                dataManager.Save(path, Project);
            }
        }

        private void BrowseOutputDirectoryCallback() {
            var dialog = new CommonOpenFileDialog();
            dialog.IsFolderPicker = true;
            CommonFileDialogResult result = dialog.ShowDialog();

            if (result == CommonFileDialogResult.Ok) {
                Project.OutputDirectory = dialog.FileName;
            }
        }

        private void GenerateCodeCallback() {
            CodeGenerator codeGenerator = new CodeGenerator();

            codeGenerator.GenerateConstantsHeader(Path.Combine(Project.OutputDirectory, "Constants.h"), Project.Constants);
            codeGenerator.GenerateMapHeader(Path.Combine(Project.OutputDirectory, "Map.h"), Project.Constants);
        }
    }
}
