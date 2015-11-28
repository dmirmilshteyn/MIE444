using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;
using Tweak.Bluetooth;
using Windows.Devices.Enumeration;
using Windows.UI.Xaml.Controls;

namespace Tweak
{
    class BluetoothViewModel : ObservableObject
    {
        BluetoothManager bluetoothManager;
        public BluetoothManager BluetoothManager {
            get { return bluetoothManager; }
            set {
                bluetoothManager = value;
                RaisePropertyChanged();
            }
        }

        DeviceInformation selectedDevice;
        public DeviceInformation SelectedDevice {
            get { return selectedDevice; }
            set {
                selectedDevice = value;
                RaisePropertyChanged();
            }
        }

        string connectButtonText;
        public string ConnectButtonText {
            get { return connectButtonText; }
            set {
                connectButtonText = value;
                RaisePropertyChanged();
            }
        }

        ConnectedDevice connectedDevice;
        public ConnectedDevice ConnectedDevice {
            get { return connectedDevice; }
            set {
                connectedDevice = value;
                RaisePropertyChanged();
            }
        }

        string dataToSend;
        public string DataToSend {
            get { return dataToSend; }
            set {
                dataToSend = value;
                RaisePropertyChanged();
            }
        }

        public ICommand SearchCommand { get; private set; }
        public ICommand ConnectCommand { get; private set; }
        public ICommand SendCommand { get; private set; }

        public BluetoothViewModel() {
            BluetoothManager = new BluetoothManager();

            SearchCommand = new Command(SearchCallback);
            ConnectCommand = new Command(ConnectCallback);
            SendCommand = new Command(SendCallback);

            SearchCallback();

            ConnectButtonText = "Connect";
        }

        private async void SendCallback() {
            await ConnectedDevice.Synchronizer.Send(DataToSend);

            DataToSend = "";
        }

        public async void SearchCallback() {
            await bluetoothManager.Search();

            if (selectedDevice == null && bluetoothManager.AvailableDevices.Count > 0) {
                SelectedDevice = bluetoothManager.AvailableDevices[0];
            }
        }

        private async void ConnectCallback() {
            if (ConnectedDevice == null) {
                try {
                    ConnectedDevice = await bluetoothManager.Connect(SelectedDevice);

                    ConnectButtonText = "Disconnect";
                } catch {
                    ConnectButtonText = "Failed... try again";
                    ConnectedDevice = null;
                }
            } else {
                ConnectedDevice.Socket.Dispose();
                ConnectedDevice = null;

                ConnectButtonText = "Connect";
            }
        }
    }
}
