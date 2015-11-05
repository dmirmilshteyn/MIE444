using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;
using Windows.Storage;
using Windows.Storage.Pickers;

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

        StorageFolder outputDirectory;
        public StorageFolder OutputDirectory {
            get { return outputDirectory; }
            set {
                outputDirectory = value;
                RaisePropertyChanged();
            }
        }

        public ICommand LoadDataFileCommand { get; private set; }
        public ICommand SaveDataFileCommand { get; private set; }
        public ICommand ImportMapCommand { get; private set; }
        public ICommand BrowseOutputDirectoryCommand { get; private set; }
        public ICommand GenerateCodeCommand { get; private set; }

        public MainViewModel() {
            Project = new Project();

            this.LoadDataFileCommand = new Command(LoadDataFileCallback);
            this.SaveDataFileCommand = new Command(SaveDataFileCallback);
            this.BrowseOutputDirectoryCommand = new Command(BrowseOutputDirectoryCallback);
            this.GenerateCodeCommand = new Command(GenerateCodeCallback);
            this.ImportMapCommand = new Command(ImportMapCallback);
        }

        private async void LoadDataFileCallback() {
            FileOpenPicker filePicker = new FileOpenPicker();
            filePicker.FileTypeFilter.Add(".xml");
            StorageFile storageFile = await filePicker.PickSingleFileAsync();
            if (storageFile != null) {
                DataManager dataManager = new DataManager();
                Project = await dataManager.Load(storageFile);
                Project.Initialize();
            }
        }

        private async void SaveDataFileCallback() {
            FileSavePicker filePicker = new FileSavePicker();
            filePicker.DefaultFileExtension = ".xml";
            filePicker.FileTypeChoices.Add("XML", new List<string>() { ".xml" });
            StorageFile storageFile = await filePicker.PickSaveFileAsync();
            if (storageFile != null) {
                DataManager dataManager = new DataManager();
                await dataManager.Save(storageFile, Project);
            }
        }

        private async void BrowseOutputDirectoryCallback() {
            FolderPicker folderPicker = new FolderPicker();
            folderPicker.FileTypeFilter.Add("*");
            StorageFolder storageFolder = await folderPicker.PickSingleFolderAsync();
            if (storageFolder != null) {
                OutputDirectory = storageFolder;
            }
        }

        private async void GenerateCodeCallback() {
            CodeGenerator codeGenerator = new CodeGenerator();

            await codeGenerator.GenerateConstantsHeader(OutputDirectory, Project.Constants);
            await codeGenerator.GenerateMapHeader(OutputDirectory, Project.Constants, project.Map);
        }

        private async void ImportMapCallback() {
            FileOpenPicker filePicker = new FileOpenPicker();
            filePicker.FileTypeFilter.Add(".png");
            StorageFile storageFile = await filePicker.PickSingleFileAsync();
            if (storageFile != null) {
                MapLoader mapLoader = new MapLoader();

                Project.Map = await mapLoader.LoadMapFromImage(storageFile, Project.Constants);
            }
        }
    }
}
